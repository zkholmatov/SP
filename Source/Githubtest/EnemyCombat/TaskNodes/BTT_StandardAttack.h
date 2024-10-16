
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

	UFUNCTION()
	void FinishAttackTask();

	UPROPERTY(EditAnywhere, Category = "Charge")
	UAnimMontage* ChargeMontage;
	
	int AttackCounter;
	int MaxAttacks;
	
protected:
	virtual void OnGameplayTaskActivated(UGameplayTask & Task) override {};

private:
	AAIController* ControllerRef;
	ACharacter* CharacterRef;
	UBehaviorTreeComponent* CachedOwnerComp;

	FTimerHandle AttackTimerHandle; // Timer handle for attack finish

	
};