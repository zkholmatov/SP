// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_TaskNodeCompleted.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_TaskNodeCompleted::UBTS_TaskNodeCompleted()
{
	NodeName = "Task Completed Toggle";
	Interval = 0.1f;
}

// void UBTS_TaskNodeCompleted::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
// 	
// 	// Toggle the TaskNodeCompleted value
// 	TaskNodeCompleted(OwnerComp);
// }

void UBTS_TaskNodeCompleted::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Toggle the TaskNodeCompleted value
	TaskNodeCompleted(OwnerComp);
}

void UBTS_TaskNodeCompleted::TaskNodeCompleted(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{	
		// Get the current value of TaskNodeCompleted from the Blackboard
		bool CurrentValue = BlackboardComp->GetValueAsBool(TEXT("TaskNodeCompleted"));

		// Toggle the value
		bool NewValue = !CurrentValue; // Switch the current value
		BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), NewValue);
	}
}
