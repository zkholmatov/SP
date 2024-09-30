// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize components here
    // E.g., CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Start a timer to destroy the projectile after 5 seconds if it does not hit the player
    GetWorldTimerManager().SetTimer(
        DestroyTimerHandle,
        this,
        &AEnemyProjectile::DestroyProjectile,
        5.0f,
        false
    );
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
    APawn* PawnRef { Cast<APawn>(OtherActor) };

    if (!PawnRef || !PawnRef->IsPlayerControlled()) {
        return;
    }

    // UE_LOG(LogTemp, Warning, TEXT("Overlapped with player!"));

    // Ensuring ParticleComponent and MovementComponent are valid before use
    if (UParticleSystemComponent* ParticleComponent = FindComponentByClass<UParticleSystemComponent>())
    {
        ParticleComponent->SetTemplate(HitParticle);
    }

    if (UProjectileMovementComponent* MovementComponent = FindComponentByClass<UProjectileMovementComponent>())
    {
        MovementComponent->StopMovementImmediately();
    }
    
    // Clear the timer since it hit the player, so the projectile won’t be destroyed after 5 seconds
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);

    FTimerHandle DeathTimerHandle{};

    GetWorldTimerManager().SetTimer(
        DeathTimerHandle,
        this,
        &AEnemyProjectile::DestroyProjectile,
        .05f
    );

    if (UParticleSystemComponent* ParticleComponent = FindComponentByClass<UParticleSystemComponent>())
    {
        ParticleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    FDamageEvent ProjectileDamageEvent{};

    PawnRef->TakeDamage(
        Damage,
        ProjectileDamageEvent,
        PawnRef->GetController(),
        this
    );
}

void AEnemyProjectile::DestroyProjectile()
{
    Destroy();
}



