// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Githubtest/Enemy/MyEnemy.h"
#include "BTT_EnAttack.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_EnAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_EnAttack();
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishTask(UBehaviorTreeComponent* OwnerComp);
private:

	UPROPERTY()
	AMyEnemy* EnemyReference;

	FTimerHandle TimerHandle;
};
