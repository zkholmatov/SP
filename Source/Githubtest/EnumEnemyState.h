// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumEnemyState.generated.h"

UENUM(BlueprintType)
enum EnumEnemyState
{
	
	Idle UMETA(DisplayName = "Idle"), // Must have comma after each enum state
	Range UMETA(DisplayName = "Range"),
	Charge UMETA(DisplayName = "Charge"),
	MeleeOne UMETA(DisplayName = "MeleeOne")
	
};
