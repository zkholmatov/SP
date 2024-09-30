// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "AIController.h"
#include "Githubtest/MyEnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Githubtest/EnumEnemyState.h"
#include "Githubtest/MyEnemyAnimInstance.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_ChasePlayer::UBTT_ChasePlayer()
{
    NodeName = TEXT("Chase Player");
    bNotifyTick = true;
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    OriginalWalkSpeed = 100.0f;
    ChaseWalkSpeed = 500.0f; // Or any appropriate speed
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!CharacterRef)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get CharacterRef in ExecuteTask."));
        return EBTNodeResult::Failed;
    }

    ControllerRef = OwnerComp.GetAIOwner();
    CachedOwnerComp = &OwnerComp;

    if (UMyEnemyAnimInstance* AnimInstance = Cast<UMyEnemyAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance()))
    {
        AnimInstance->bIsChasing = true;// Assuming bIsChasing is a boolean property in AnimInstance
    }

    OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;
    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChaseWalkSpeed;

    ChasePlayer(OwnerComp);

    return EBTNodeResult::InProgress;
}

void UBTT_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (!PlayerRef || !CharacterRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    uint8 CurrentState = CachedOwnerComp->GetBlackboardComponent()->GetValueAsEnum(TEXT("CurrentState"));
    if (CurrentState != static_cast<uint8>(EnumEnemyState::Chase))
    {
        FinishTask(OwnerComp);
    }

    ChasePlayer(OwnerComp);

    // Turn to face the player
    FVector DirectionToPlayer = (PlayerRef->GetActorLocation() - CharacterRef->GetActorLocation()).GetSafeNormal();
    FRotator NewRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
    CharacterRef->SetActorRotation(NewRotation);
}

void UBTT_ChasePlayer::ChasePlayer(UBehaviorTreeComponent& OwnerComp)
{
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!PlayerRef || !ControllerRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector PlayerLocation = PlayerRef->GetActorLocation();
    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAcceptanceRadius(75.0f);

    FPathFollowingRequestResult Result = ControllerRef->MoveTo(MoveRequest);
    if (Result.Code == EPathFollowingRequestResult::Failed)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
}

void UBTT_ChasePlayer::FinishTask(UBehaviorTreeComponent& OwnerComp)
{
    if (UMyEnemyAnimInstance* AnimInstance = Cast<UMyEnemyAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance()))
    {
        AnimInstance->bIsChasing = false; // Reset the chase animation state
    }

    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}