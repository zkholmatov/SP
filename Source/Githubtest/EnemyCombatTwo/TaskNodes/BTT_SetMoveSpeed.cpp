// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetMoveSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"


UBTT_SetMoveSpeed::UBTT_SetMoveSpeed()
{
	NodeName = "Set AI Speed";
	NewSpeed = 150.0f; // Default speed
}

EBTNodeResult::Type UBTT_SetMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	UCharacterMovementComponent* MovementComponent = AICharacter->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = NewSpeed;
	
	return EBTNodeResult::Succeeded;
}
