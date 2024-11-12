// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_V2ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"

// Constructor for the Chase Player behavior tree task node.
UBTT_V2ChasePlayer::UBTT_V2ChasePlayer()
{
    NodeName = TEXT("Chase Player (Not State Based)");
    bNotifyTick = true;
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;

    // AcceptanceRadius = 100.0f;  
}

// Executes the task.
EBTNodeResult::Type UBTT_V2ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Initialize and validate references
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    ControllerRef = OwnerComp.GetAIOwner();
    CachedOwnerComp = &OwnerComp;

    if (!CharacterRef || !ControllerRef || !CachedOwnerComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get required references in ExecuteTask."));
        return EBTNodeResult::Failed;
    }

    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAcceptanceRadius(AcceptableRadius);

    if (ControllerRef->MoveTo(MoveRequest).Code == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    // Set focus to the player
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (PlayerRef)
    {
        ControllerRef->SetFocus(PlayerRef);
    }

    return EBTNodeResult::InProgress;
}

// Called every tick while the task is running.
void UBTT_V2ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!GetWorld()->GetFirstPlayerController()->GetPawn() || !CharacterRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAcceptanceRadius(AcceptableRadius);

    if (ControllerRef->MoveTo(MoveRequest).Code == EPathFollowingRequestResult::Failed)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    float DistanceBetweenActors = FVector::Dist(PlayerLocation, CharacterRef->GetActorLocation());
    if (DistanceBetweenActors <= AcceptableRadius + 25)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

// Function to clean up after the chase task is finished.
void UBTT_V2ChasePlayer::FinishTask(UBehaviorTreeComponent& OwnerComp)
{
    // Clear focus when the task is finished
    if (ControllerRef)
    {
        ControllerRef->ClearFocus(EAIFocusPriority::Gameplay);
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
