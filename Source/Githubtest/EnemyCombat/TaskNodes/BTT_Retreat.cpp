// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Retreat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Githubtest/Enemy/EnumEnemyState.h"
#include "Engine/World.h"



UBTT_Retreat::UBTT_Retreat()
{
	NodeName = TEXT("Retreating");
	bNotifyTick = false;
	bCreateNodeInstance = true;

	ControllerRef = nullptr;
	CharacterRef = nullptr;
	CachedOwnerComp = nullptr;
	RetreatMontage = nullptr;
}

EBTNodeResult::Type UBTT_Retreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Cache owner and character for further usage
	CachedOwnerComp = &OwnerComp;
	// casts the pawn controlled by the AI owner of the behavior tree
	// component OwnerComp to the ACharacter class and assigns it to the variable CharacterRef
	CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!CharacterRef)
	{
		return EBTNodeResult::Failed;
	}
    
	// Ensure we have a valid animation montage
	if (RetreatMontage)
	{
		// Play the retreat montage
		ControllerRef = OwnerComp.GetAIOwner();
		CharacterRef->PlayAnimMontage(RetreatMontage);

		// Set up a timer to complete the task after the animation duration
		CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
		RetreatTimerHandle, [this]()
		{
			FinishRetreatTask();
		},
		RetreatMontage->GetPlayLength(), false);
        
		return EBTNodeResult::InProgress;
	}
    
	return EBTNodeResult::Failed;
}

void UBTT_Retreat::FinishRetreatTask()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CachedOwnerComp is null"));
	}
}
