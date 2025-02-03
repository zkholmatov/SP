// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = 1; // Set all enemies to Team ID 1, anyone who isnt an enemy on screen will be considered player or neutral

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
	//  BlackboardComp = GetController<AAIController>()->GetBlackboardComponent(); // Causing bug?
	// BlackboardComp->SetValueAsEnum( 
	//     TEXT("CurrentState"), 
	//     InitialState // Check details panel of enemyManny to see whta initial state is currently set to 
	// );
}


// // Called every frame
// void AMyEnemy::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }


// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void AMyEnemy::SetTaskNodeCompleted(bool TaskNodeCompleted) // THis is only used to for special edge cases, revert to BTS_TaskNodeCompleted service for inside BT
{
	if (BlackboardComp)
	{	
		BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), TaskNodeCompleted);
		if (TaskNodeCompleted == true)
		{
			BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), false);
		}
		else
		{
			BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), true);
		}
	}
}

// Implement the DestroyEnemy function
void AMyEnemy::DestroyEnemy()
{
	Destroy();
}

FGenericTeamId AMyEnemy::GetGenericTeamId() const
{
	return FGenericTeamId(TeamId);
}

void AMyEnemy::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	TeamId = NewTeamId.GetId();
}
