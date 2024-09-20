// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LookAtPlayerAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API ULookAtPlayerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin( USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
		) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
		) override;
};
