// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//BlackboardComp = GetController<AAIController>().GetBlackboardComponent();
	BlackboardComp = GetController<AAIController>()->GetBlackboardComponent();
	
	BlackboardComp->SetValueAsEnum( 
	    TEXT("CurrentState"), 
	    InitialState // Check details panel of enemyManny to see whta initial state is currently set to 
	);
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// DetectPawn - Determines if the specified pawn should be detected and updates the AI state accordingly.
// DetectedPawn - The pawn that has been detected by this enemy.
// PawnToDetect - The specific pawn that should trigger a change in state.

void AMyEnemy::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	// Retrieve the current state of the enemy from the Blackboard Component.
	// The state is initially set based on the InitialState variable and may be updated during gameplay.
	// Current state is technically variable created with in black board
	EnumEnemyState CurrentState = static_cast<EnumEnemyState>
	(BlackboardComp->GetValueAsEnum(TEXT("CurrentState"))); 

	// Check if the detected pawn is not the specific pawn we are interested in, or if the current state is not Idle.
	// If either condition is true, exit the function as we are not interested in detecting this pawn or the enemy is busy in another state.
	if (DetectedPawn != PawnToDetect || CurrentState != EnumEnemyState::Idle)
	{
		return;
	}

	// If the conditions above are not met, this means the enemy is currently idle and has detected the specific pawn 
	// Log a message to the console for debugging purposes (commented out here).
	// UE_LOG(LogTemp, Warning, TEXT("Player Detected!"));
	
	// Update the Blackboard Component to change the enemy's state to 'Chase'.
	// This triggers behavior associated with the Chase state, such as pursuing the detected pawn.
	BlackboardComp->SetValueAsEnum( 
		TEXT("CurrentState"), 
		EnumEnemyState::Chase // needs to be swapped to Chase
	);
}

// Implement the SetEnemyState function
void AMyEnemy::SetEnemyState(EnumEnemyState NewState)
{
	if (BlackboardComp)
	{	
		BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), NewState);
		if (NewState == EnumEnemyState::Death)
		{
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMyEnemy::DestroyEnemy, 5.0f, false);
		}
	}
}

// Implement the DestroyEnemy function
void AMyEnemy::DestroyEnemy()
{
	Destroy();
}










