// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumEnemyState.h"
#include "MyEnemy.generated.h"

UCLASS()
class GITHUBTEST_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

	// Variable to store the initial state of the enemy, editable in the Unreal Editor
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EnumEnemyState> InitialState;

	// Pointer to the Blackboard Component used for AI state management
	class UBlackboardComponent* BlackboardComp;

public:
	// Sets default values for this character's properties
	AMyEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	// Custom event that can be overridden in Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void MySwordTraceEvent();

	// Custom event that can be overridden in Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void MySwordTraceLoopEvent();

	// Custom event that can be overridden in Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void MySwordTraceStopEvent();
};
