// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Retreat.generated.h"

UCLASS()
class GITHUBTEST_API UBTT_Retreat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Retreat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishRetreatTask();

	UPROPERTY(EditAnywhere, Category = "Range")
	UAnimMontage* RetreatMontage;

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override {};

	UFUNCTION()
	void LookAtPlayer();

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	FTimerHandle RetreatTimerHandle;
	
};