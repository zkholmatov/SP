// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_TaskNodeCompleted.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTS_TaskNodeCompleted : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_TaskNodeCompleted();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	// virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void TaskNodeCompleted(UBehaviorTreeComponent& OwnerComp);
};
