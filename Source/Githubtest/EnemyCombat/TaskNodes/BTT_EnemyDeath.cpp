// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyDeath.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Githubtest/EnumEnemyState.h"


UBTT_EnemyDeath::UBTT_EnemyDeath()
{
    NodeName = "Enemy Death";
    bNotifyTick = false; // Disable tick notification
    bCreateNodeInstance = true;
    DeathMontage = nullptr;
}

EBTNodeResult::Type UBTT_EnemyDeath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // UE_LOG(LogTemp, Warning, TEXT("Success!"));
    ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};

    if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; };
    CharacterRef->PlayAnimMontage(DeathMontage);
	
    return EBTNodeResult::Succeeded;
}
