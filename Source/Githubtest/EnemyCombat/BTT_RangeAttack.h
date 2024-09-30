// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
    
public:
	UBTT_RangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishAttackTask();

	UPROPERTY(EditAnywhere, Category = "Range")
	UAnimMontage* RangeMontage;

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override {};

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	FTimerHandle AttackTimerHandle; // Timer handle for attack finish
};

