// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class GITHUBTEST_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FLatentActionInfo LatentInfo;

public:
	// // Called every frame
	// virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Moving Platform")
	float AllowedDistance = 0;
	// FVector PlatformVelocity = FVector(-200, 0, 0);

	UPROPERTY()
	USceneComponent* DefaultRoot;

	// Static mesh platform component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	UStaticMeshComponent* StaticMeshPlatform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Movement")
	float DurationOfMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Movement")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Movement")
	FRotator TargetRotation;

	FVector StartLocation;

	UFUNCTION(BlueprintCallable, category = "Platform")
	void ActorMovement();

	UFUNCTION(BlueprintCallable, category = "Platform")
	void ActorMovementReturn();

};
