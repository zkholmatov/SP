// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPlayerAnimInstance.generated.h"

//NOT CURRENTLY IN USE AS OF 9/30/2024

/**
 * UMyPlayerAnimInstance - Animation instance class for the player 
 * character. Manages animation state and updates animations according
 * to player velocity.
 */
UCLASS()
class GITHUBTEST_API UMyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// Stores the current velocity of the player. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	float currentVelocity{0.0f};

	// Function to update the current velocity of the player. 
	UFUNCTION(Blueprintcallable)
	void UpdateVelocity();
};
