// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCCombatNotifyState.h"
#include "Githubtest/MyEnemy.h"

   void UMyCCombatNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceEvent();
   	}
   }

void UMyCCombatNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
   {
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MySwordTraceStopEvent();
   	}
   }
