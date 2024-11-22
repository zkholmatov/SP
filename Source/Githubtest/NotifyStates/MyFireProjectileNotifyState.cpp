// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFireProjectileNotifyState.h"
#include "Githubtest/Enemy/MyEnemy.h"

void UMyFireProjectileNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
   	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
   	{
   		Enemy->MyFireProjectileEvent();
   	}
}

// void UMyFireProjectileNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
// {
// 	if (AMyEnemy* Enemy = Cast<AMyEnemy>(MeshComp->GetOwner()))
// 	{
// 		Enemy->MyFireProjectileStopEvent();
// 	}
// }