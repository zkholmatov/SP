// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingEnvironment.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARotatingEnvironment::ARotatingEnvironment()
{
    // Enable ticking
    PrimaryActorTick.bCanEverTick = true;

    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    // Create the static mesh component
    RotatingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatingMesh"));
    RotatingMesh->SetupAttachment(RootComponent); // Attach to the default root
    RotatingMesh->SetRelativeLocation(FVector::ZeroVector);
    RotatingMesh->SetMobility(EComponentMobility::Movable);

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
    CollisionBox->SetupAttachment(RotatingMesh);

}

// Called when the game starts or when spawned
void ARotatingEnvironment::BeginPlay()
{
    Super::BeginPlay();

    InitialRotation = RotatingMesh->GetRelativeRotation();

    // Bind overlap events
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARotatingEnvironment::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ARotatingEnvironment::OnOverlapEnd);

}



// Handles the overlap event
void ARotatingEnvironment::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlap triggered by: %s"), *OtherActor->GetName()));

    // This makes a super fast rotation no blend add me to overlap
    //// Set a timer to call RotateMesh repeatedly
    //GetWorld()->GetTimerManager().SetTimer(
    //    RotationTimerHandle,                    // Timer handle to manage the timer
    //    this,                                   // The object to call the function on
    //    &ARotatingEnvironment::RotateMesh,                  // Function to call
    //    RotationTime,                       // Time interval in seconds
    //    true                                    // Loop the timer
    //);


    if (OtherActor && OtherActor != this && OtherComp)
    {
        UE_LOG(LogTemp, Log, TEXT("Overlapped with that %s"), *OtherActor->GetName());
        bShouldRotate = true; // Start rotation
        SetActorTickEnabled(true);
    }
}

void ARotatingEnvironment::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlap NO LONGER triggered by: %s"), *OtherActor->GetName()));
        bShouldReturn = true; // Start rotation
        SetActorTickEnabled(true);
    }
}


// Called every frame
void ARotatingEnvironment::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldRotate)
    {
        // Interpolate smoothly to the target rotation
        CurrentRotation = RotatingMesh->GetRelativeRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationTime);
        RotatingMesh->SetRelativeRotation(NewRotation);

        // Stop rotating once close enough to the target rotation
        if (CurrentRotation.Equals(TargetRotation, 0.1f)) // Adjust tolerance as needed
        {
            bShouldRotate = false;
            //SetActorTickEnabled(false);
        }
    }
    else if (bShouldReturn) {
        // Interpolate smoothly back to the initial rotation
        CurrentRotation = RotatingMesh->GetRelativeRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, InitialRotation, DeltaTime, RotationTime);
        RotatingMesh->SetRelativeRotation(NewRotation);

        // Stop returning once close enough to the initial rotation
        if (CurrentRotation.Equals(InitialRotation, 0.1f)) // Adjust tolerance as needed
        {
            bShouldReturn = false;
            SetActorTickEnabled(false); // Disable ticking if no other rotations are needed
        }
    }
}

// This makes a super fast rotation no blend add me to overlap
    //// Set a timer to call RotateMesh repeatedly
    //GetWorld()->GetTimerManager().SetTimer(
    //    RotationTimerHandle,                    // Timer handle to manage the timer
    //    this,                                   // The object to call the function on
    //    &ARotatingEnvironment::RotateMesh,                  // Function to call
    //    RotationTime,                       // Time interval in seconds
    //    true                                    // Loop the timer
    //);

// void ARotatingEnvironment::RotateMesh()
// {
//    
//     if (!RotatingMesh)
//     {
//         // Debug message for null RotatingMesh
//         if (GEngine)
//         {
//             GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RotatingMesh is null!"));
//         }
//         return;
//     }
//
//
//     GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Rotating: Yaw: %f, Pitch: %f, Roll: %f"), YawValue, PitchValue, RollValue));
//     // Get the current relative rotation
//     FRotator CurrentRotation = RotatingMesh->GetRelativeRotation();
//
//     // Add the rotation amount to the Yaw (Y-axis rotation)
//     CurrentRotation.Yaw += YawValue;
//     CurrentRotation.Pitch += PitchValue;
//     CurrentRotation.Roll += RollValue;
//
//     // Apply the new rotation
//     RotatingMesh->SetRelativeRotation(CurrentRotation);
//     
// }