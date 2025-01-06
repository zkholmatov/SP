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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	APawn* PossessedPawn;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightSenseConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Damage* DamageSenseConfig;
	
	TSubclassOf<UAISense_Sight> DominantSense;

	void InitPerceptionConfig();
	// virtual void OnPossess(APawn* InPawn) override;

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

	UFUNCTION() // NO need for this ot be called in event graph call extending functions 
	void OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus const& Stimulus);
	
	// UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	// void OnPossessExtended(APawn* InPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void BeginPlayExtended();

	//************************CPP and extensions for found and lost *******************************//
	UFUNCTION()
	void HandleSensed(AActor* PlayerActor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Perception")
	void OnActorFound(AActor* LostActor);

	UFUNCTION()
	void HandleLostSense(AActor* PlayerActor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Perception")
	void OnActorLost(AActor* LostActor);
	
	//*********************************************************************//
	// ------------------- For State Functionality ------------------------// 
	UBlackboardComponent* GetBlackboardComp();

	UFUNCTION(BlueprintCallable, Category = "AI")
	uint8 GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsIdle();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStateAsChase();
};

