// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnemyStateService.generated.h"

UCLASS()
class GITHUBTEST_API UEnemyStateService : public UBTService
{
	GENERATED_BODY()
    
public:
	UEnemyStateService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	const float ChargeDistance = 250.0f;
	const float ChaseDistance = 1250.0f;

	void UpdateEnemyState(UBehaviorTreeComponent& OwnerComp);

	
};
