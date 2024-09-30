// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtPlayerAnimNotifyState.h"
#include "LookAtPlayerComponent.h"

void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* OwnerRef { MeshComp->GetOwner() };

	if (!IsValid(OwnerRef)) { return; }

	ULookAtPlayerComponent* RotationComp { OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	if (!IsValid(RotationComp)) { return; }

	RotationComp->bCanRotate = true;
}

void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerRef { MeshComp->GetOwner() };

	if (!IsValid(OwnerRef)) { return; }

	ULookAtPlayerComponent* RotationComp { OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	if (!IsValid(RotationComp)) { return; }

	RotationComp->bCanRotate = false;
}

