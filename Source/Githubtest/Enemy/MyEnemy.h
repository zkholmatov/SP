// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumEnemyState.h"
#include "GenericTeamAgentInterface.h"
#include "MyEnemy.generated.h"


/**
 * AMyEnemy - Character class representing an enemy in the game. Manages 
 * AI state and interactions with player characters.
 */
UCLASS()
class GITHUBTEST_API AMyEnemy : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	// Variable to store the initial state of the enemy, editable in the Unreal Editor, used to set the enemy's initial behavior state
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EnumEnemyState> InitialState; // variable is type EnumEnemyState

	// Pointer to the Blackboard Component used for AI state management, stores AI-relevant information such as the current state
	class UBlackboardComponent* BlackboardComp;

	FTimerHandle DestroyTimerHandle;

public:
	// Sets default values for this character's properties
	AMyEnemy();

	// Component to manage the enemy's static mesh, editable and readable in the Unreal Editor, used for rendering the enemy's appearance
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Team ID variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	uint8 TeamId;

	// IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;  
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;

public:	
	// Called every frame to update the enemy's state
	// virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isLocked { false };

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// // Function to detect another pawn, used to change the enemy's state based on proximity to other characters
	// UFUNCTION(BlueprintCallable)
	// void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	// Custom event that can be overridden in Blueprints, used to handle sword trace events
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon Trace")
	void MySwordTraceEvent();

	// Custom event that can be overridden in Blueprints, used to handle stopping of sword trace events
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "Weapon Trace")
	void MySwordTraceStopEvent();
	
	// Custom event that can be overridden in Blueprints
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "Projectile")
    void MyFireProjectileEvent();

	// // Custom event that can be overridden in Blueprints
	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "Projectile")
	// void MyFireProjectileStopEvent();

	UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }

	// Function to set the enemy state
	UFUNCTION(BlueprintCallable)
	void SetEnemyState(EnumEnemyState NewState);

	// Function to set the enemy state
	UFUNCTION(BlueprintCallable)
	void SetTaskNodeCompleted(bool TaskNodeCompleted);

	/** Function to destroy the enemy */
	void DestroyEnemy();
	
	

};
