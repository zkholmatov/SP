// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtPlayerAnimNotifyState.h"
#include "Githubtest/LookAtPlayer/LookAtPlayerComponent.h"


void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// Retrieve the owner actor of the skeletal mesh component
	AActor* OwnerRef { MeshComp->GetOwner() };

	// Check if the owner is valid, exit if not
	if (!IsValid(OwnerRef)) { return; }

	// Find the ULookAtPlayerComponent attached to the owner actor
	ULookAtPlayerComponent* RotationComp { OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	// Check if the LookAtPlayerComponent is valid, exit if not
	if (!IsValid(RotationComp)) { return; }

	// Enable the rotation component to allow the actor to rotate towards the player
	RotationComp->bCanRotate = true;
}


void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// Super::NotifyEnd(MeshComp, Animation, EventReference);

	// Retrieve the owner actor of the skeletal mesh component
	AActor* OwnerRef { MeshComp->GetOwner() };

	// Check if the owner is valid, exit if not
	if (!IsValid(OwnerRef)) { return; }

	// Find the ULookAtPlayerComponent attached to the owner actor
	ULookAtPlayerComponent* RotationComp { OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	// Check if the LookAtPlayerComponent is valid, exit if not
	if (!IsValid(RotationComp)) { return; }

	// Disable the rotation component to stop the actor from rotating towards the player
	RotationComp->bCanRotate = false;
}

