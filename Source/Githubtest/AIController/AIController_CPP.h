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

	// Perception Component  and config variables 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> CppPerceptionComponent;

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

public:

	UFUNCTION() // NO need for this to be called in event graph call extending functions 
	void OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void BeginPlayExtended();

	UPROPERTY()
	UAISenseConfig_Sight* SightSenseConfig;
	
	UPROPERTY()
	UAISenseConfig_Damage* DamageSenseConfig;

	UFUNCTION(blueprintCallable, Category = "AI")
	void InitPerceptionConfig();

	//************************CPP and extensions for found and lost *******************************//
	UFUNCTION()
	void HandleSensed(AActor* PlayerActor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnActorFound(AActor* LostActor);

	UFUNCTION()
	void HandleLostSense(AActor* PlayerActor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
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