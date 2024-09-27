// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RangeAttack.h"
#include "AIController.h"
#include"GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Githubtest/EnumEnemyState.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};
	
	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; };
	
	// Get the Blackboard Component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) ==
		static_cast<uint8>(EnumEnemyState::Range))
	{
		if (CharacterRef)  // Ensure CharacterRef is valid
		{
			// Perform Range Attack
			CharacterRef->PlayAnimMontage(AnimMontage);
	
			// // Set the CurrentState to Range Attack
			// OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
			// 	TEXT("CurrentState"), EnumEnemyState::Charge);
	
			// Return Succeeded as the task result
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
	
}