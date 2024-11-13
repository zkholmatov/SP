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

    AcceptableRadius = 185.0f;
}

// Executes the task.
EBTNodeResult::Type UBTT_V2ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Initialize and validate references
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    ControllerRef = OwnerComp.GetAIOwner();
    CachedOwnerComp = &OwnerComp;

    ControllerRef->ClearFocus(EAIFocusPriority::Gameplay);

    if (!CharacterRef || !ControllerRef || !CachedOwnerComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get required references in ExecuteTask."));
        return EBTNodeResult::Failed;
    }

    // Start chasing the player
    ChasePlayer(OwnerComp);
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

    // Check if AI has reached the player within the acceptable radius
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    float DistanceBetweenActors = FVector::Dist(PlayerLocation, CharacterRef->GetActorLocation());
    // UE_LOG(LogTemp, Log, TEXT("Distance to Player: %f"), DistanceBetweenActors);
    if (DistanceBetweenActors < AcceptableRadius + 45.0f)
    {
        // UE_LOG(LogTemp, Log, TEXT("AcceptableRadius: %f"), AcceptableRadius);
        // UE_LOG(LogTemp, Error, TEXT("Radius reached "));
        FinishTask(OwnerComp);
        return;
    }
    
    ChasePlayer(OwnerComp); // Keep running son
}

// Function to make the AI chase the player.
void UBTT_V2ChasePlayer::ChasePlayer(UBehaviorTreeComponent& OwnerComp)
{
    APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    
    FAIMoveRequest MoveRequest{ PlayerLocation };
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAcceptanceRadius(AcceptableRadius);
    
    // Move to player location and set focus
    ControllerRef->MoveTo(MoveRequest);
    // ControllerRef->SetFocus(PlayerRef);
}

// Function to clean up after the chase task is finished.
void UBTT_V2ChasePlayer::FinishTask(UBehaviorTreeComponent& OwnerComp)
{
    // Clear focus when the task is finished and notify the behavior tree
    if (ControllerRef)
    {
        // ControllerRef->ClearFocus(EAIFocusPriority::Gameplay);
        ControllerRef->StopMovement();
    }

    // UE_LOG(LogTemp, Error, TEXT("OMG! CAN I GET YOU TO SIGN THIS PETITION?"));
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}