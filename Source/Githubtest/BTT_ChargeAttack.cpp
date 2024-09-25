// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChargeAttack.h"
#include "AIController.h"
#include "MyEnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnumEnemyState.h"


void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (GEngine)
	{
		FString Status = bIsFinished ? TEXT("Finished: True") : TEXT("Finished: False");
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, Status);
	}
	
	bool bIsReadyToCharge{
		OwnerComp.GetBlackboardComponent()
		->GetValueAsBool(TEXT("IsReadyToCharge"))
	};

	if(bIsReadyToCharge)
	{
		OwnerComp.GetBlackboardComponent()
		->SetValueAsBool(TEXT("IsReadyToCharge"), false);

		ChargeAtPlayer();
	}

	if(!bIsFinished){ return; }

	ControllerRef->ReceiveMoveCompleted.Remove(MoveCompletedDelegate);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	MoveCompletedDelegate.BindUFunction(
		this,
		"HandleMoveCompleted"
	);
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = ControllerRef->GetCharacter();
	EnemyAnimInstance = Cast<UMyEnemyAnimInstance>( CharacterRef->GetMesh()->GetAnimInstance() );

	EnemyAnimInstance->isCharging = true;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		TEXT("IsReadyToCharge"),
		false
		);

	bIsFinished = false;
	
	return EBTNodeResult::InProgress;
	
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	// UE_LOG(LogTemp, Warning, TEXT("Charging Player"));
	APawn* PlayerRef{GetWorld()->GetFirstPlayerController()->GetPawn()};
	FVector PlayerLocation = PlayerRef->GetActorLocation();

	FAIMoveRequest MoveRequest{ PlayerLocation };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);

	ControllerRef->MoveTo(MoveRequest);
	ControllerRef->SetFocus(PlayerRef);

	ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompletedDelegate);

	OriginalWalkSpeed = CharacterRef->GetCharacterMovement()
	->MaxWalkSpeed;

	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
}

void UBTT_ChargeAttack::HandleMoveCompleted()
{
	EnemyAnimInstance->isCharging = false;

	FTimerHandle AttackTimerHandle;

	CharacterRef->GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&UBTT_ChargeAttack::FinishAttackTask,
		1.0f,
		false
		);

	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	// UE_LOG(LogTemp, Warning, TEXT("Task finished")); // this is a log check!!!! don't delete me
	bIsFinished = true;
}
