// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIController_CPP.generated.h"

UCLASS()
class GITHUBTEST_API AAIController_CPP : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIController_CPP(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> CppPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* PossessedPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float sightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float loseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float peripheralVisionAngleDeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float maxAge;
	
	
	
protected:
	virtual void BeginPlay() override;
	
private:
	
	UAISenseConfig_Sight* SightSenseConfig;
	UAISenseConfig_Damage* DamageSenseConfig;
	
	TSubclassOf<UAISense_Sight> DominantSense;

	void InitPerceptionComponent();
	
	
public:

	// This is for super advanced modification to the perception update
	// UFUNCTION()
	// void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	// This is for advanced modification to the perception update
	// UFUNCTION()
	// virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	// Only necessary if there are different things being done for each different sense
	// UFUNCTION()
	// bool CanSenseActor(AActor* SensedActor, FName SenseName, const FAISenseID& StimulusType);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus const Stimulus);

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleSensed(AActor* PlayerActor);

	UFUNCTION()
	void HandleLostSense(AActor* PlayerActor);

	UFUNCTION(BlueprintCallable, Category = "AI")
	uint8 GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsIdle();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsChase();
};
