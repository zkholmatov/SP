// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetStaticMoveSpeed.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"


UBTT_SetStaticMoveSpeed::UBTT_SetStaticMoveSpeed()
{
	NodeName = "Set Static Move Speed";
}

EBTNodeResult::Type UBTT_SetStaticMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* AICharacter = Cast<ACharacter>(AIOwner->GetPawn());
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (bReturnToOriginalSpeed)
	{
		OriginalSpeed = BlackboardComponent->GetValueAsFloat(BlackboardKey.SelectedKeyName);
		UCharacterMovementComponent* MovementComponent = AICharacter->GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = OriginalSpeed;
	}
	else 
	{
		UCharacterMovementComponent* MovementComponent = AICharacter->GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = NewSpeed;
	}
	
	
	return EBTNodeResult::Succeeded;
}
