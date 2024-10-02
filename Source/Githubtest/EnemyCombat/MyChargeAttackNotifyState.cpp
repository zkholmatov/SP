// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChargeAttackNotifyState.h"
#include "Githubtest/MyEnemy.h"

   void UMyChargeAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceEvent();
   	}
   }

void UMyChargeAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceStopEvent();
   	}
   }