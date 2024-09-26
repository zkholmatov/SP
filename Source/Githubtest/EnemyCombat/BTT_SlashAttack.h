// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SlashAttack.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_SlashAttack : public UBTTaskNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
