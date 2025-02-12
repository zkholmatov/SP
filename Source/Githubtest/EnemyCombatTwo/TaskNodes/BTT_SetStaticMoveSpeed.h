// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetStaticMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_SetStaticMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetStaticMoveSpeed();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	FBlackboardKeySelector BlackboardKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float NewSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bReturnToOriginalSpeed;
	
	float OriginalSpeed;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
