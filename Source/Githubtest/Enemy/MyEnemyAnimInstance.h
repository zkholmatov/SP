// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyEnemyAnimInstance.generated.h"

/**
 * UMyEnemyAnimInstance - Animation instance class for the enemy character. 
 * Manages animation state variables such as whether the enemy is chasing 
 * or attacking.
 */
UCLASS()
class GITHUBTEST_API UMyEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// // Stores the current velocity of the enemy. Editable and readable
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	// float currentVelocity{0.0f};

public:
	// // Indicates whether the enemy is currently chasing the player
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	// bool bIsChasing{ false };
	
	// // Indicates whether the enemy is currently attacking the player
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	// bool bIsAttacking{ false };

	// // Indicates whether the enemy is currently hesitating  
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	// bool bIsHesitating{ false };
};
