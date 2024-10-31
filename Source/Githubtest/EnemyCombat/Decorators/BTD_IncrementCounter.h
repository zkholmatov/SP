// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IncrementCounter.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTD_IncrementCounter : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IncrementCounter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	FBlackboardKeySelector Counter;

	// UPROPERTY(BlueprintReadOnly, Category = "Count")
	// int MaxCount;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
