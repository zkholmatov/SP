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

// Constructor for the Chase Player behavior tree task node.
UBTT_ChasePlayer::UBTT_ChasePlayer()
{
    // Set the node name for debugging purposes.
    NodeName = TEXT("Chase Player");
    
    // Notify the tree every tick.
    bNotifyTick = true;
    
    // Create an instance of this node.
    bCreateNodeInstance = true;

    // Initialize references to nullptr.
    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    
    // Set default walk speeds.
    OriginalWalkSpeed = 100.0f;
    ChaseWalkSpeed = 250.0f; // This is a max walk speed
}

// Executes the task.
EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get and cache the character controlled by this AI.
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!CharacterRef)
    {
        // Log an error and fail the task if no character found.
        UE_LOG(LogTemp, Error, TEXT("Failed to get CharacterRef in ExecuteTask."));
        return EBTNodeResult::Failed;
    }

    // Cache references for later use.
    ControllerRef = OwnerComp.GetAIOwner();
    CachedOwnerComp = &OwnerComp;

    // Set the animation state to chasing if the AnimInstance is valid.
    if (UMyEnemyAnimInstance* AnimInstance = Cast<UMyEnemyAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance()))
    {
        AnimInstance->bIsChasing = true; // Assuming bIsChasing is a boolean property in AnimInstance
    }

    // Store the original walk speed and set the character's walk speed to chase speed.
    OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;
    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChaseWalkSpeed;

    // Initiate chasing the player.
    ChasePlayer(OwnerComp);

    // Indicate that the task is still in progress.
    return EBTNodeResult::InProgress;
}

// Called every tick while the task is running.
void UBTT_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // Reference to the player character.
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();

    // If either reference is invalid, fail the task.
    if (!PlayerRef || !CharacterRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Check the current state from the blackboard.
    uint8 CurrentState = CachedOwnerComp->GetBlackboardComponent()->GetValueAsEnum(TEXT("CurrentState"));
    
    // If the state is not chasing, finish the task.
    if (CurrentState != static_cast<uint8>(EnumEnemyState::Chase))
    {
        FinishTask(OwnerComp);
    }

    // Continue chasing the player.
    ChasePlayer(OwnerComp);

    // // Turn to face the player.
    // FVector DirectionToPlayer = (PlayerRef->GetActorLocation() - CharacterRef->GetActorLocation()).GetSafeNormal();
    // FRotator NewRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
    // CharacterRef->SetActorRotation(NewRotation);
}

// Function to initiate chasing the player.
void UBTT_ChasePlayer::ChasePlayer(UBehaviorTreeComponent& OwnerComp)
{
    // Reference to the player character.
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    
    // If either reference is invalid, fail the task.
    if (!PlayerRef || !ControllerRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Get the player's location.
    FVector PlayerLocation = PlayerRef->GetActorLocation();
    
    // Create a move request.
    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAcceptanceRadius(75.0f); 

    // Attempt to move towards the player.
    FPathFollowingRequestResult Result = ControllerRef->MoveTo(MoveRequest);
    
    // If the move fails, fail the task.
    if (Result.Code == EPathFollowingRequestResult::Failed)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
}

// Function to clean up after the chase task is finished.
void UBTT_ChasePlayer::FinishTask(UBehaviorTreeComponent& OwnerComp)
{
    // Set the animation state to not chasing if the AnimInstance is valid.
    if (UMyEnemyAnimInstance* AnimInstance = Cast<UMyEnemyAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance()))
    {
        AnimInstance->bIsChasing = false; // Reset the chase animation state
    }

    // Restore the original walk speed.
    CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
    
    // Indicate the task has succeeded.
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}