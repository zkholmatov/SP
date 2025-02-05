// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicFloor.h"
#include "Engine/StaticMeshActor.h"
#include "TimerManager.h"



// Sets default values
ADynamicFloor::ADynamicFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsCircle = false;
}

// Called when the game starts or when spawned
void ADynamicFloor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADynamicFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADynamicFloor::InitSpawnNextActor(UStaticMesh* InputStaticMesh)
{
	if (!InputStaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("StaticMesh is null. Cannot spawn floor."));
		return;
	}

	StaticMesh = InputStaticMesh;

	// Start the timer for spawning
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle, 
		this, 
		&ADynamicFloor::SpawnNextActor,
		SpawnRate, 
		true
	);
}

void ADynamicFloor::SpawnNextActor()
{
    if (!StaticMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("StaticMesh is null. Cannot spawn actor."));
        return;
    }

    if (ActorsSpawned >= MaxActorsToSpawn)
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        UE_LOG(LogTemp, Log, TEXT("Maximum number of actors spawned."));
        return;
    }

	FVector SpawnLocation;

	if (bIsCircle) {
		float Angle = ActorsSpawned * 2.0f * PI / MaxActorsToSpawn; // Angle in radians for this actor
		SpawnLocation = GetActorLocation() + FVector(Radius * FMath::Cos(Angle), Radius * FMath::Sin(Angle), 0.0f);
	}
	else {
		SpawnLocation = GetActorLocation() + FVector(ActorsSpawned * X_DistanceOffset, ActorsSpawned * Y_DistanceOffset, 0.0f);

	}
   
    // Get parameters for setting spawn attributes
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AStaticMeshActor* NewActor = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnLocation, MeshRotation, SpawnParams);
    if (NewActor)
    {
        NewActor->SetMobility(EComponentMobility::Movable);

        UStaticMeshComponent* MeshComponent = NewActor->GetStaticMeshComponent();
        if (MeshComponent)
        {
            MeshComponent->SetStaticMesh(StaticMesh);
            MeshComponent->SetWorldScale3D(FVector(MeshScaleX, MeshScaleY, MeshScaleZ));
        }

        ArrayFloor.Push(NewActor);
        ActorsSpawned++;

        UE_LOG(LogTemp, Log, TEXT("Spawned actor %d at location: %s"), ActorsSpawned, *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor."));
    }
}

void ADynamicFloor::SpawnGrid(UStaticMesh* InputStaticMesh)
{
	if (!InputStaticMesh) { // Check if mesh is valid 
		UE_LOG(LogTemp, Warning, TEXT("InputStaticMesh is null. Cannot spawn floor."));
		return;
	}

	// Get intitial location of actor in world, ie static mesh. Where ever Bp is dropped will be location
	const FVector IntitialSpawnLocation = GetActorLocation();

	FBoxSphereBounds Bounds = InputStaticMesh->GetBounds(); // Get bounds of mesh 
	// get bounds of x axis to set distance between spawn locations, 
	// if meshs are not a perfect square this may need to be changed to a differnt axis and toggles with a set bool
	float NextMeshDistance = Bounds.BoxExtent.X * 2 + X_DistanceOffset; 

	// Get Paramaeters for setting spawn attributes
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // Set owner of spawn, ie this BP that has this class as a parent

	for (int row = 0; row < rowValue; row++) {
		for (int column = 0; column < columnValue; column++) {
			FVector spawnLocation = IntitialSpawnLocation + FVector(row * NextMeshDistance, column * NextMeshDistance, 0);

			// instantiate a new Static mesh actor and Spawn actor in world based upon new location, rotation and Spawn parmameters 
			AStaticMeshActor* NewFloor = GetWorld()->SpawnActor<AStaticMeshActor>(spawnLocation, MeshRotation, SpawnParams);

			if (NewFloor)
			{
				//Set New floor instance attributes mesh component, is movable, and scale 
				NewFloor->GetStaticMeshComponent()->SetStaticMesh(InputStaticMesh);
				NewFloor->SetMobility(EComponentMobility::Movable);
				NewFloor->GetStaticMeshComponent()->SetWorldScale3D(FVector(MeshScaleX, MeshScaleY, MeshScaleZ));

				// ----------------Add new floor instance to array, may end up using teh array for something, if not delete array-------------------------------
				ArrayFloor.Push(NewFloor);
			}
		}
	}
		
}