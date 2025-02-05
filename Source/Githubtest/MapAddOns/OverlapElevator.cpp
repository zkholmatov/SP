#include "OverlapElevator.h"
//#include "Components/BoxComponent.h"
//#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

// Constructor
AOverlapElevator::AOverlapElevator()
{
    PrimaryActorTick.bCanEverTick = false; // No need for Tick when using MoveComponentTo

    // Initialize components
    ElevatorPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorPlatform"));
    RootComponent = ElevatorPlatform;

    ElevatorPlatform->SetMobility(EComponentMobility::Movable);

    CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
    CollisionBoxComponent->SetupAttachment(RootComponent);

    // Initialize variables
    //TargetHeight = 500.0f; // Default height
    bIsMoving = false;
    bIsAtTop = false;
}

// BeginPlay
void AOverlapElevator::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap events
    CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AOverlapElevator::OnOverlapBegin);
    CollisionBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AOverlapElevator::OnOverlapEnd);

    
    StartLocation = ElevatorPlatform->GetComponentLocation();
    TargetLocation = StartLocation + FVector(0.0f, 0.0f, TargetHeight);
}


void AOverlapElevator::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsMoving)
    {
        bIsMoving = true;

        // Determine target based on the current state
        FVector Target;

        if (bIsAtTop)
        {
            Target = StartLocation; // Move down
        }
        else
        {
            Target = TargetLocation; // Move up
        }

        // Move the elevator
        MoveElevator(Target);
    }
}

void AOverlapElevator::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    
}

// Move elevator function
void AOverlapElevator::MoveElevator(const FVector& Target)
{
    FLatentActionInfo LatentInfo; // See documentation page for reference 
    LatentInfo.CallbackTarget = this;

    // Use MoveComponentTo for smooth movement
    UKismetSystemLibrary::MoveComponentTo(
        ElevatorPlatform,
        Target,
        FRotator::ZeroRotator,  // No rotation change
        true,                   // Ease in
        true,                   // Ease out
        3.0f,                   // Duration 
        false,                  // Not teleporting
        EMoveComponentAction::Move,  // Action type
        LatentInfo
    );


    // Delay to handle post-movement actions
   /* GetWorldTimerManager().SetTimerForNextTick([this, Target]()
        {
            OnElevatorMovementComplete();
        });*/
    FTimerHandle MovementCompleteHandle;
    GetWorldTimerManager().SetTimer(MovementCompleteHandle, this, &AOverlapElevator::OnElevatorMovementComplete, 1.0, false);
}

// Elevator movement complete
void AOverlapElevator::OnElevatorMovementComplete()
{
    bIsMoving = false;
    bIsAtTop = !bIsAtTop;
    //bIsAtTop = (ElevatorPlatform->GetComponentLocation() == TargetLocation);
    
}