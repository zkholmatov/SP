// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CloseCombat.h"
#include "AIController.h"
#include"GameFramework/Character.h"

UBTT_CloseCombat::UBTT_CloseCombat()
{
	NodeName = "Close Combat";
	bNotifyTick = false; // Disable tick notification
	bCreateNodeInstance = true;
	CloseCombatMontage = nullptr;
}

EBTNodeResult::Type UBTT_CloseCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// UE_LOG(LogTemp, Warning, TEXT("Success!"));
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; };
	CharacterRef->PlayAnimMontage(CloseCombatMontage);
	
	return EBTNodeResult::Succeeded;
}
