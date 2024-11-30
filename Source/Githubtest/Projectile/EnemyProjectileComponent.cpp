// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectileComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UEnemyProjectileComponent::UEnemyProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyProjectileComponent::SpawnProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass)
{
	USceneComponent* SpawnPointComp {Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ComponentName))};

	FVector SpawnLocation {SpawnPointComp->GetComponentLocation()};

	FVector PLayerLocation { GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() };

	FRotator SpawnRotation { UKismetMathLibrary::FindLookAtRotation(SpawnLocation,PLayerLocation) };

	GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation);
}

void UEnemyProjectileComponent::SpawnProjectileFromSocket(FName SocketName, TSubclassOf<AActor> ProjectileClass)
{
	
	UStaticMeshComponent* SpawnPointComp {Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(SocketName))};
	// Get the socket transform
	// FTransform SocketTransform = SpawnPointComp->GetSocketTransform(SocketName);

	// FVector SpawnLocation = SocketTransform.GetLocation();
	FVector SpawnLocation = SpawnPointComp->GetSocketLocation(SocketName);
	
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerLocation);

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
}


