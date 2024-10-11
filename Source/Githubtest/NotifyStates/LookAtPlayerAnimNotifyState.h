// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LookAtPlayerAnimNotifyState.generated.h"

// ULookAtPlayerAnimNotifyState - An animation notify state that triggers the actor to look at the player
// Inherits from UAnimNotifyState to allow integration with the animation system
UCLASS()
class GITHUBTEST_API ULookAtPlayerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	// NotifyBegin is called when the animation state begins
	// MeshComp - Skeletal mesh component acting upon
	// Animation - The animation sequence that triggered this notify
	// TotalDuration - Duration for which the animation notify state will be active
	// EventReference - Reference to the animation notify event
	virtual void NotifyBegin( USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	// NotifyEnd is called when the animation state ends
	// MeshComp - Skeletal mesh component acting upon
	// Animation - The animation sequence that triggered the end of this notify
	// EventReference - Reference to the animation notify event
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};