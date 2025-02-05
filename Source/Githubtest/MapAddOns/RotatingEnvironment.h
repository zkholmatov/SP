// Fill out your copyright notice in the Description page of Project Settings.
// /* https://github.com/Harrison1/unrealcpp/blob/master/OpenDoorWithLerp/OpenDoorWithLerp.cpp  */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RotatingEnvironment.generated.h"


UCLASS()
class GITHUBTEST_API ARotatingEnvironment : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARotatingEnvironment();

    // Rotation parameters
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    // float PitchValue;
    //
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    // float YawValue;
    //
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    // float RollValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    FRotator TargetRotation;
    
    // Mesh and collision components
    UPROPERTY()
    USceneComponent* DefaultRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* RotatingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
    float RotationTime;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // UFUNCTION()
    // void RotateMesh();
    FTimerHandle RotationTimerHandle;
    

private:
    bool bShouldRotate = false; // Tracks whether rotation is active
    bool bShouldReturn = false;

    FRotator InitialRotation;
    FRotator CurrentRotation;

    // Handles overlap begin event
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};




