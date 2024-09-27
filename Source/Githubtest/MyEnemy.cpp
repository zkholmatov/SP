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
	    InitialState
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

void AMyEnemy::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	// Setting initial state correctly
	EnumEnemyState CurrentState = static_cast<EnumEnemyState>
	(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")));

	if (DetectedPawn != PawnToDetect || CurrentState != EnumEnemyState::Idle)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Player Detected!"));
	
	BlackboardComp->SetValueAsEnum( 
        TEXT("CurrentState"), 
        EnumEnemyState::Chase
    );
}






