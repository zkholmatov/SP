// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Retreat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Githubtest/Enemy/EnumEnemyState.h"
#include "Engine/World.h"


UBTT_Retreat::UBTT_Retreat()
{
    NodeName = TEXT("Retreat");
    bNotifyTick = true; 
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    RetreatMontage = nullptr;
}

EBTNodeResult::Type UBTT_Retreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    

    if (!InitializeReferences(OwnerComp))
    {
        return EBTNodeResult::Failed;
    }

    // retrieves the blackboard component associated with the behavior tree component OwnerComp, which manages the execution of this task, and assigns it to the variable BlackboardComp
    // ownerComp is a reference to the UBehaviorTreeComponent that owns and manages the execution of this particular behavior tree task node
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent(); 

    if (ShouldRetreat(BlackboardComp))
    {
        PlayRetreatMontage();
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

bool UBTT_Retreat::InitializeReferences(UBehaviorTreeComponent& OwnerComp)
{
    CachedOwnerComp = &OwnerComp;
    // casts the pawn controlled by the AI owner of the behavior tree component OwnerComp to the ACharacter class and assigns it to the variable CharacterRef
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    return CharacterRef != nullptr;
}

bool UBTT_Retreat::ShouldRetreat(UBlackboardComponent* BlackboardComp) const
{
    return BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Retreat)
        && RetreatMontage;
}

void UBTT_Retreat::PlayRetreatMontage()
{
    // assigns the AI owner of CachedOwnerComp to ControllerRef and then calls PlayAnimMontage on CharacterRef to play the animation montage
    ControllerRef = CachedOwnerComp->GetAIOwner();
    CharacterRef->PlayAnimMontage(RetreatMontage);

    CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
        RetreatTimerHandle, [this]()
        {
            FinishRetreatTask();
        },
        RetreatMontage->GetPlayLength(), false);
}

void UBTT_Retreat::FinishRetreatTask()
{
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
