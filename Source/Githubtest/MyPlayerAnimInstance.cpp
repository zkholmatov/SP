// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerAnimInstance.h"

// Function to update the player's velocity
void UMyPlayerAnimInstance::UpdateVelocity()
{
    // Retrieve a reference to the pawn ie the player character 
    APawn* PawnRef{ TryGetPawnOwner() };

    // If the pawn reference is invalid  exit the function
    if(!IsValid(PawnRef)) return;

    // Get the current velocity vector of the pawn
    FVector Velocity { PawnRef->GetVelocity() };

    // Convert the velocity vector length (magnitude) to a float and assign it to the current velocity
    currentVelocity = static_cast<float>( Velocity.Length() );
}