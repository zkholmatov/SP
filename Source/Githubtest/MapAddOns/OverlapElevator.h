#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OverlapElevator.generated.h"

UCLASS()
class GITHUBTEST_API AOverlapElevator : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AOverlapElevator();

    // Collision component for overlap detection
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    UBoxComponent* CollisionBoxComponent;

    // Elevator platform component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    UStaticMeshComponent* ElevatorPlatform;

    // Maximum height the elevator should reach
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TargetHeight;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    FVector StartLocation;   // Initial position of the elevator
    FVector TargetLocation;  // Target position the elevator will reach
    bool bIsMoving;          // Tracks if the elevator is currently moving
    bool bIsAtTop;           // Tracks if the elevator is at the top position

    // Event handlers for overlap
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Function to move the elevator
    void MoveElevator(const FVector& Target);

    // Callback for when the elevator finishes moving
    void OnElevatorMovementComplete();
};