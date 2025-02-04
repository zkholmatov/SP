// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_SetMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetMoveSpeed();
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float MinSpeed;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float SpeedLimit;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
