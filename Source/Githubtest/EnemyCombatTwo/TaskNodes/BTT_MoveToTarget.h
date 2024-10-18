// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveToTarget();

	UPROPERTY(EditAnywhere, Category = "VARIABLES")
	float AcceptanceRadius = 200.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
