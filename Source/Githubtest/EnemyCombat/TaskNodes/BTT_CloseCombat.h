// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CloseCombat.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_CloseCombat : public UBTTaskNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	UAnimMontage* CloseCombatMontage;
public:
	UBTT_CloseCombat();
	
protected:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
