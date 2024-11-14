// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTraceNotifyState.h"
#include "Githubtest/Enemy/MyEnemy.h"

   void UWeaponTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceEvent();
   	}
   }

void UWeaponTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceStopEvent();
   	}
   }
