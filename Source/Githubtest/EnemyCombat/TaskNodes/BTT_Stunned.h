// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Stunned.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_Stunned : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Stunned();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishAttackTask();
	
	UPROPERTY(EditAnywhere, Category = "Charge")
	UAnimMontage* StunnedMontage;

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	FTimerHandle StunnedTimerHandle; // Timer handle for attack finish
};
