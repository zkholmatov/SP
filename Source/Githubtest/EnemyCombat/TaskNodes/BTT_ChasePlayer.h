// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChasePlayer.generated.h"

// UBTT_ChasePlayer class inherits from UBTTaskNode, providing functionality for an AI to chase the player.
UCLASS()
class GITHUBTEST_API UBTT_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ChasePlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	void ChasePlayer(UBehaviorTreeComponent& OwnerComp);
	void FinishTask(UBehaviorTreeComponent& OwnerComp);

private:
	bool InitializeReferences(UBehaviorTreeComponent& OwnerComp);
	FVector GetPlayerLocation();
	bool IsTaskValid();

	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	float OriginalWalkSpeed;
	float ChaseWalkSpeed;
};