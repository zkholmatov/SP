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
	// Constructor to initialize default values and settings for the task.
	UBTT_ChasePlayer();

	// Executes the task when the behavior tree reaches this node.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	// Called every tick while the task is running.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// Function to initiate chasing the player.
	void ChasePlayer(UBehaviorTreeComponent& OwnerComp);
	
	// Function to clean up and finish the task.
	void FinishTask(UBehaviorTreeComponent& OwnerComp);

private:
	// Reference to the AI controller managing the character.
	AAIController* ControllerRef;
	
	// Reference to the character being controlled by the AI.
	ACharacter* CharacterRef;
	
	// Cached reference to the behavior tree component running this task.
	UBehaviorTreeComponent* CachedOwnerComp;

	// Original walk speed stored for resetting after chase.
	float OriginalWalkSpeed;
	
	// Walk speed to use while chasing the player.
	float ChaseWalkSpeed;
};