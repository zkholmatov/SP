
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StandardAttack.generated.h"

UCLASS()
class GITHUBTEST_API UBTT_StandardAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_StandardAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishAttackTask();

	UFUNCTION()
	void HandleMontageCompleted(FName NotifyName);

	UFUNCTION()
	void HandleMontageInterrupted(FName NotifyName);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackMontage;

	// UPROPERTY(EditAnywhere, Category = "Attack")
	// UAnimInstance* Test;

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	FTimerHandle AttackTimerHandle; // Timer handle for attack finish
};