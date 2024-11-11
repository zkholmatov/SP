// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IncrementCounter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTD_IncrementCounter::UBTD_IncrementCounter()
{
	NodeName = "Increment Attack Counter";
	// MaxCount = FMath::RandRange(4,7);
}

bool UBTD_IncrementCounter::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// MaxCount = FMath::RandRange(4,7);
	int const MaxCount  = FMath::RandRange(min,max);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent)
	{
		int CurrentCount = BlackboardComponent->GetValueAsInt(Counter.SelectedKeyName);
		// CurrentCount++;
		BlackboardComponent->SetValueAsInt(Counter.SelectedKeyName, CurrentCount + 1);
		if(CurrentCount >= MaxCount)
		{
			BlackboardComponent->SetValueAsInt(Counter.SelectedKeyName, 0);
		}
	}

	return true;
}
