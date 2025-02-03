// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EngineUtils.h" // for iterator 
#include "Githubtest/Enemy/MyEnemy.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIController_CPP.generated.h"

UCLASS()
class GITHUBTEST_API AAIController_CPP : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIController_CPP(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<AMyEnemy*> EngagedEnemies;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* FoundActor;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// virtual void PostInitializeComponents() override;
	
private:
	FTimerHandle ForgetPlayerTimerHandle;

public:
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/ai-perception-in-unreal-engine
	UFUNCTION() // NO need for this to be called in event graph call extending functions 
	void OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void BeginPlayExtended();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightSenseConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Damage* DamageSenseConfig;

	UFUNCTION(blueprintCallable, Category = "AI")
	void InitPerceptionConfig();

	// UFUNCTION(BlueprintImplementableEvent, Category = "AI|Setup")
	// void InitializeAIConfigInEditor();

	//************************CPP and extensions for found and lost *******************************//
	UFUNCTION()
	void HandleSensed(AActor* PlayerActor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnActorFoundCPP(AActor* LostActor);

	UFUNCTION()
	void HandleLostSense();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnActorLostCPP(AActor* LostActor);

	UFUNCTION()
	void HandleOnDamage(AActor* DamagingActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnDamageSenseCPP(AActor* DamagingActor);
	
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