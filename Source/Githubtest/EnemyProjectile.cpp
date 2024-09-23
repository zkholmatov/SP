// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* PawnRef { Cast<APawn>(OtherActor) };

	if (!PawnRef->IsPlayerControlled()) { return; }

	// UE_LOG(LogTemp, Warning, TEXT("Overlapped with player!"));

	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitParticle);

	FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();
}

//void AEnemyProjectile::Destroyed()
//{
    
//}

