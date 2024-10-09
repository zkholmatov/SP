// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnemyDeath.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_EnemyDeath : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_EnemyDeath();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Death")
	UAnimMontage* DeathMontage;
protected:
	
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override {};
	
};
