// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if not needed.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	// Default root component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// Create the Platform and attach it to the root component
	StaticMeshPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	StaticMeshPlatform->SetupAttachment(RootComponent); // Attach to the default root
	StaticMeshPlatform->SetRelativeLocation(FVector::ZeroVector);
	StaticMeshPlatform->SetMobility(EComponentMobility::Movable);

	// Set default duration for the platform movement
	DurationOfMovement = 5.0f;

	// Initialize the Latent Action Info structure // found on forum 
	LatentInfo.CallbackTarget = this; // Sets this actor as the target for the callback when the action completes
	LatentInfo.Linkage = 0;           // Linkage is used to distinguish multiple latent actions; here, it's set to default (0)
	LatentInfo.UUID = FGuid::NewGuid().A; // Generates a unique identifier for this specific latent action 
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// StartLocation = GetActorLocation();
	StartLocation = StaticMeshPlatform->GetRelativeLocation();
	TargetLocation += StartLocation;

	// Start the platform movement
	ActorMovement();
}

void AMovingPlatform::ActorMovement()
{
	// Specify the function to call when the latent action completes
	LatentInfo.ExecutionFunction = FName("ActorMovementReturn");

	// Use MoveComponentTo() to smoothly move the platform
	UKismetSystemLibrary::MoveComponentTo(
		StaticMeshPlatform,                     // The component to move
		TargetLocation,         // Target location
		TargetRotation,       // Target rotation
		true,                           // Ease in
		true,                           // Ease out
		DurationOfMovement,                       // Duration of the movement
		true,                           // Use shortest rotation path
		EMoveComponentAction::Move,     // Action type
		LatentInfo                      // Latent action information
	);
}

void AMovingPlatform::ActorMovementReturn()
{
	// Reset the function to call when this movement completes
	LatentInfo.ExecutionFunction = FName("ActorMovement");

	// Restart the movement, creating a loop
	UKismetSystemLibrary::MoveComponentTo(
		StaticMeshPlatform,// Scene Component
		StartLocation, // Target Relative location
		FRotator(0.f, 0.f, 0.f), // Target Relative Rotation
		true, // Ease Out
		true, // Ease In
		DurationOfMovement, // Duration of Movement along distance 
		true, // Force shortest path
		EMoveComponentAction::Move, // Movement component type
		LatentInfo // Latent Action Info
	);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	//Moving Platform location//
		//Get current location
	FVector CurrentLocation = GetActorLocation();
		//Add vector to location/ update location within tick
	CurrentLocation += (PlatformVelocity * DeltaTime);
		//Set location
	SetActorLocation(CurrentLocation);
	//Double Platform back to origin location
		//Check platform movement distance
	float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);
		//Reverse Direction of platform if gone to far
	if (DistanceMoved > AllowedDistance) 
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation += MoveDirection * AllowedDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}
}