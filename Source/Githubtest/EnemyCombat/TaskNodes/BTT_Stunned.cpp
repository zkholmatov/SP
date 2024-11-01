// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Stunned.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Githubtest/Enemy/EnumEnemyState.h"


UBTT_Stunned::UBTT_Stunned()
{
    NodeName = "Stunned";
    bNotifyTick = false; // Disable tick notification
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    StunnedMontage = nullptr;
}

EBTNodeResult::Type UBTT_Stunned::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }

    // Get the Blackboard component
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    // BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), false);
    
    // Ensure we have a valid animation montage
    if (StunnedMontage)
    {
        // Play the stunned montage
        ControllerRef = OwnerComp.GetAIOwner();
        CharacterRef->PlayAnimMontage(StunnedMontage);
        
        // Set up a timer to complete the task after the animation duration
        CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
            StunnedTimerHandle, [this]()
            {
                // PlaySecondMontage();
                FinishAttackTask();
            },
            StunnedMontage->GetPlayLength(), false);

        return EBTNodeResult::InProgress;
    }
    

    return EBTNodeResult::Failed;
}

void UBTT_Stunned::FinishAttackTask()
{
    if (CachedOwnerComp)
    {
        UBlackboardComponent* BlackboardComp = CachedOwnerComp->GetBlackboardComponent();
        // BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), true);
        
        // BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Chase));
        
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
