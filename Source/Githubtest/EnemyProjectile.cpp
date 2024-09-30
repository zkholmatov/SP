// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize components here
    // For example, you could initialize the collision component like so:
    // CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Start a timer to destroy the projectile after 5 seconds if it does not hit the player
    GetWorldTimerManager().SetTimer(
        DestroyTimerHandle, // Timer handle to manage this timer
        this,               // The object that owns the timer
        &AEnemyProjectile::DestroyProjectile, // Function to call when the timer expires
        5.0f,               // Delay before calling the function, in seconds
        false               // Do not loop the timer
    );
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Handles the event when the projectile begins overlapping with another actor
void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
    // Cast the other actor to APawn to check if it's a player-controlled pawn
    APawn* PawnRef { Cast<APawn>(OtherActor) };

    // If the overlapping actor is not a valid pawn or not player-controlled, return early
    if (!PawnRef || !PawnRef->IsPlayerControlled()) {
        return;
    }

    // Log for debugging purposes (currently commented out)
    // UE_LOG(LogTemp, Warning, TEXT("Overlapped with player!"));

    // If the projectile has a particle system component, set the hit particle effect template
    if (UParticleSystemComponent* ParticleComponent = FindComponentByClass<UParticleSystemComponent>())
    {
        ParticleComponent->SetTemplate(HitParticle);
    }

    // If the projectile has a movement component, stop its movement immediately
    if (UProjectileMovementComponent* MovementComponent = FindComponentByClass<UProjectileMovementComponent>())
    {
        MovementComponent->StopMovementImmediately();
    }
    
    // Clear the destruction timer since the projectile has hit the player
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);

    // Set a short timer to destroy the projectile shortly after hitting the player
    FTimerHandle DeathTimerHandle{};

    GetWorldTimerManager().SetTimer(
        DeathTimerHandle,  // Handle to manage the death timer
        this,              // The object that owns the timer
        &AEnemyProjectile::DestroyProjectile, // Function to call when the timer expires
        .05f               // Delay before calling the function, in seconds
    );

    // Disable collision on the particle component if it's valid
    if (UParticleSystemComponent* ParticleComponent = FindComponentByClass<UParticleSystemComponent>())
    {
        ParticleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // // Create a damage event for the projectile
    // FDamageEvent ProjectileDamageEvent{};
    //
    // // Apply damage to the player pawn
    // PawnRef->TakeDamage(
    //     Damage,               // Amount of damage to apply
    //     ProjectileDamageEvent, // Damage event
    //     PawnRef->GetController(), // Controller responsible for the damage
    //     this                  // Actor causing the damage
    // );
}

// Destroys the projectile actor
void AEnemyProjectile::DestroyProjectile()
{
    Destroy();
}



