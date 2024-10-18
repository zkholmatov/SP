// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToTarget.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveToTarget::UBTT_MoveToTarget()
{
	NodeName = "Move To Player";
}

// EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	AAIController* AIOwner = OwnerComp.GetAIOwner();
// 	if (!AIOwner)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
//
// 	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
// 	if (!PlayerPawn)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
//
// 	// AIOwner->MoveToActor(PlayerPawn, 150.0f); // Move within 5 units of the player
// 	EPathFollowingRequestResult::Type MoveResult = AIOwner->MoveToActor(PlayerPawn, 150.0, true, true, false, 0, true);
//
// 	return EBTNodeResult::InProgress;
// }

EBTNodeResult::Type UBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return EBTNodeResult::Failed;
	}

	EPathFollowingRequestResult::Type MoveResult = AIOwner->MoveToActor(PlayerPawn, 150.0, true, true, false, 0, true);

	// Return success immediately if already at the goal
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::Succeeded;
	}
	else if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

