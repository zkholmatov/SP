// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h" // for iterator 
#include "DynamicFloor.generated.h"

UCLASS()
class GITHUBTEST_API ADynamicFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicFloor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<AStaticMeshActor*> ArrayFloor; // Needs to reference a class type to no break reference

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int rowValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int columnValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float MeshScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float MeshScaleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float MeshScaleZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FRotator MeshRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float X_DistanceOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float Y_DistanceOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRate = 1.0f; // Time in seconds between spawns

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bIsCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxActorsToSpawn = 10; // Total number of actors to spawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UStaticMesh* StaticMesh; // Store the mesh for spawning

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimerHandle; // Timer handle for spawning actors
	int ActorsSpawned = 0; // Counter for spawned actors

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnGrid(UStaticMesh* InputStaticMesh);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void InitSpawnNextActor(UStaticMesh* InputStaticMesh);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnNextActor();

};
