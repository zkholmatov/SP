// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RangeAttack.h"
#include "AIController.h"
#include"GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// #include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Githubtest/Enemy/EnumEnemyState.h"
#include "Githubtest/Enemy/MyEnemy.h"

UBTT_RangeAttack::UBTT_RangeAttack()
{
    NodeName = "Range Attack";
    bNotifyTick = false; 
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    RangeMontage = nullptr;
}

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }
    
    // Ensure we have a valid animation montage
    if (RangeMontage)
    {
        // Play the range attack montage
        ControllerRef = OwnerComp.GetAIOwner();
        CharacterRef->PlayAnimMontage(RangeMontage);

        // Set up a timer to complete the task after the animation duration
        CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
            AttackTimerHandle, [this]()
            {
                FinishAttackTask();
            },
            RangeMontage->GetPlayLength(), false);

        return EBTNodeResult::InProgress;
    }
    
    return EBTNodeResult::Failed;
}

void UBTT_RangeAttack::FinishAttackTask()
{
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}