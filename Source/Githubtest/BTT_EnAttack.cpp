// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

// UBTT_EnAttack::UBTT_EnAttack()
// {
// 	NodeName = "Attack";
// 	bNotifyTick = false; // Disable tick notification
// 	bCreateNodeInstance = true;
// }
//
// EBTNodeResult::Type UBTT_EnAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	// return Super::ExecuteTask(OwnerComp, NodeMemory);
//
// 	// Get the AI controller
// 	AAIController* AIController = OwnerComp.GetAIOwner();
// 	if (!AIController)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
//
// 	// Get the controlled pawn and cast it to AMyEnemy
// 	EnemyReference = Cast<AMyEnemy>(AIController->GetPawn());
// 	if (!EnemyReference)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
// 	
// 	UAnimMontage* AttackMontage = EnemyReference->AttackMontage; // Get the attack montage from the EnemyReference
// 	if (!AttackMontage)
// 	{
// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("NOT VALID MONTAGE %d")));
// 		}
// 		return EBTNodeResult::Failed;
// 	}
//
// 	// EnemyReference->EnemyAttack(AttackMontage);
//
// 	//FinishTask function to the animation completion event
// 	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &OwnerComp]()
// 	{
// 		FinishTask(&OwnerComp);
// 		
// 	}, AttackMontage->GetPlayLength(), false);
//
// 	return EBTNodeResult::InProgress;
// }
//
// void UBTT_EnAttack::FinishTask(UBehaviorTreeComponent* OwnerComp)
// {
// 	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
// }

