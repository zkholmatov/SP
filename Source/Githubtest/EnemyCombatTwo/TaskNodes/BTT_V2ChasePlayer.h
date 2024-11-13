// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_V2ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UBTT_V2ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_V2ChasePlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Radius")
	float AcceptableRadius;

protected:
	void ChasePlayer(UBehaviorTreeComponent& OwnerComp);
	void FinishTask(UBehaviorTreeComponent& OwnerComp);

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;
	
};
