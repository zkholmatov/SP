// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UMyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	float currentVelocity{0.0f};

	UFUNCTION(Blueprintcallable)
	void UpdateVelocity();
};
