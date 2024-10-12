// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStateService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Githubtest/Enemy/EnumEnemyState.h"

UEnemyStateService::UEnemyStateService()
{
	NodeName = TEXT("Update Enemy State");
	Interval = 0.1f;
}

void UEnemyStateService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UpdateEnemyState(OwnerComp);
}

void UEnemyStateService::UpdateEnemyState(UBehaviorTreeComponent& OwnerComp)
{
	ACharacter* CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	if (!IsValid(CharacterRef)) {
		UE_LOG(LogTemp, Error, TEXT("CharacterRef is not valid!"));
		return;
	}

	APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!IsValid(PlayerRef)) {
		UE_LOG(LogTemp, Error, TEXT("PlayerRef is not valid!"));
		return;
	}

	float DistanceToPlayer = FVector::Dist(PlayerRef->GetActorLocation(), CharacterRef->GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("Distance to Player: %f"), DistanceToPlayer);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) {
	    UE_LOG(LogTemp, Error, TEXT("BlackboardComp is not valid!"));
	    return;
	}

	bool isRetreating = false;
	

	// Get the current state from the blackboard
	EnumEnemyState CurrentState = static_cast<EnumEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")));
	if (CurrentState != EnumEnemyState::Retreat)
	{
		isRetreating = true;
	}

	// Update state only if the current state is not 'Death'
	if (CurrentState != EnumEnemyState::Death || /* NOT RETREATING*/!isRetreating)
	{
		if (CurrentState != EnumEnemyState::Retreat && DistanceToPlayer <= ChargeDistance )
		{
			// UE_LOG(LogTemp, Log, TEXT("Setting State to Charge"));
			
			BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Charge));
		}
		else if (CurrentState != EnumEnemyState::Retreat && DistanceToPlayer <= ChaseDistance)
		{
			// UE_LOG(LogTemp, Log, TEXT("Setting State to Chase"));
			
			BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Chase));
		}
		else if (DistanceToPlayer > ChargeDistance + 5)
		{
			// UE_LOG(LogTemp, Log, TEXT("Setting State to Idle"));
			
			BlackboardComp->SetValueAsEnum(TEXT("CurrentState" ), static_cast<uint8>(EnumEnemyState::Idle));
		}
		// else
		// {
		// 	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Retreat));
		// }
	}
}