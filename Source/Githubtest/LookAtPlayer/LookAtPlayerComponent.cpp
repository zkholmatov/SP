// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtPlayerComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Call the base class version of TickComponent to ensure any inherited functionality is preserved
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bCanRotate) {return;}
	
	// Retrieve a reference to enemy
	AActor* OwnerRef{ GetOwner() };

	// Get the location of the enemy ie owner ref
	FVector OwnerLocation = OwnerRef->GetActorLocation();

	// Retrieve the first player controller in the world
	APlayerController* PlayerController {GetWorld()->GetFirstPlayerController()};

	// Get the player's pawn 
	APawn* PlayerPawn { PlayerController->GetPawn() };
	// Get the location of the player's pawn
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	// Calculate the rotation that would be needed for the owner to look directly at the player
	FRotator DesiredRotation {UKismetMathLibrary::FindLookAtRotation(OwnerLocation, PlayerLocation)};

	// Get the current rotation of the enemy
	FRotator CurrentRotation { OwnerRef->GetActorRotation() };

	// Make rotation a constant speed
	FRotator NewRotation {
		UKismetMathLibrary::RInterpTo_Constant(
			CurrentRotation,
			DesiredRotation,
			DeltaTime,
			speed)
	};

	// Preserve pitch and roll and apply only the yaw rotation for smoother turning
	FRotator NewYawOnlyRotator { CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll };

	// Set the new rotation to the owner actor
	OwnerRef->SetActorRotation(NewYawOnlyRotator);
}