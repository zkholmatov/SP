// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GITHUBTEST_API UMyEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variables")
	float currentVelocity{0.0f};

public:
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "variables")
	bool bIsChasing{ false };

	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "variables")
	bool bIsAttacking{ false };
};
