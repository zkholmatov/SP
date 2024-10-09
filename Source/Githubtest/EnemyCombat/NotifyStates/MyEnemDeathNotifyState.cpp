// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemDeathNotifyState.h"
#include "Githubtest/MyEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Githubtest/EnumEnemyState.h"

void UMyEnemDeathNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// Cast the owner of the mesh component to AMyEnemy to ensure we are dealing with an enemy character
	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
	{
		Enemy->MySwordTraceStopEvent();
		// Access the Blackboard component using the getter method
		if (UBlackboardComponent* BlackboardComp = Enemy->GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Death));
		}
	}
}

void UMyEnemDeathNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Cast the owner of the mesh component to AMyEnemy to ensure we are dealing with an enemy character
	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
	{
		// Access the Blackboard component using the getter method
		if (UBlackboardComponent* BlackboardComp = Enemy->GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Death));
		}
	}
}
