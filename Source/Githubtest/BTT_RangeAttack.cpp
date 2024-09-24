// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RangeAttack.h"
#include "AIController.h"
#include"GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnumEnemyState.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// // UE_LOG(LogTemp, Warning, TEXT("Success!"));
	// ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};
	//
	// if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; };
	//
	// CharacterRef->PlayAnimMontage(AnimMontage);
	//
	// double RandomValue{ UKismetMathLibrary::RandomFloat() }; // Returns random number between 0 and 1
	//
	// if ( RandomValue > Threshold)
	// {
	// 	Threshold = 0.9;
	//
	// 	OwnerComp.GetBlackboardComponent()
	// 	->SetValueAsEnum( TEXT("CurrentState"), EnumEnemyState::Charge );
	//
	// 	// UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// 	// if (BlackboardComp)
	// 	// {
	// 	// 	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Charge));
	// 	// }
	// 	
	// 	// UE_LOG(LogTemp, Warning, TEXT("Charging player!")); // this is a log check!!!! don't delete me 
	// }
	// else
	// {
	// 	Threshold -= 0.1;
	// }
	//
	// return EBTNodeResult::Succeeded;

	// Get AI and Player references
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};
	APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!IsValid(CharacterRef) || !IsValid(PlayerRef))
	{
		return EBTNodeResult::Failed;
	}

	// Get the distance between AI and Player
	float DistanceToPlayer = FVector::Dist(CharacterRef->GetActorLocation(), PlayerRef->GetActorLocation());

	// Print distance to the screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Distance to Player: %f"), DistanceToPlayer));
	}
	
	// Define the distance threshold for Charge
	float ChargeAttackDistance = 750.0f;

	// Check if the player is within Charge Attack distance
	if (DistanceToPlayer < ChargeAttackDistance)
	{
		// Set the CurrentState to Charge if within 500 units
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EnumEnemyState::Charge);
	}
	else
	{
		// Perform Range Attack if player is farther than 500 units
		CharacterRef->PlayAnimMontage(AnimMontage);

		// Set the CurrentState to Range Attack
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EnumEnemyState::Range);
	}

	// Succeed the task as the appropriate state has been set
	return EBTNodeResult::Succeeded;
}