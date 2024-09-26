// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SlashAttack.h"
#include "AIController.h"
#include"GameFramework/Character.h"

EBTNodeResult::Type UBTT_SlashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// UE_LOG(LogTemp, Warning, TEXT("Success!"));
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; };
	CharacterRef->PlayAnimMontage(AnimMontage);
	
	return EBTNodeResult::Succeeded;
}
