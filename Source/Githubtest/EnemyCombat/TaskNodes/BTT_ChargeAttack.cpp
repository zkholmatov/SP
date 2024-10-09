


#include "BTT_ChargeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Githubtest/EnumEnemyState.h"


UBTT_ChargeAttack::UBTT_ChargeAttack()
{
    NodeName = "Charge Attack";
    bNotifyTick = false; // Disable tick notification
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    ChargeMontage = nullptr;
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef){ return EBTNodeResult::Failed; }

    // Get the Blackboard component
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // Verify the state is correctly set to Charge
    if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Charge))
    {
        // Ensure we have a valid animation montage
        if (ChargeMontage)
        {
            // Play the charge attack montage
            ControllerRef = OwnerComp.GetAIOwner();
            CharacterRef->PlayAnimMontage(ChargeMontage);

            // // Fire sword trace event if applicable
            // AMyEnemy* MyEnemyRef = Cast<AMyEnemy>(CharacterRef);
            // if (MyEnemyRef)
            // {
            //     MyEnemyRef->MySwordTraceEvent();
            // }

            // Set up a timer to complete the task after the animation duration
            CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
                AttackTimerHandle, [this]()
                {
                    FinishAttackTask();
                },
                ChargeMontage->GetPlayLength(), false);

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
    // if (!CachedOwnerComp)
    // {
    //     return;
    // }
    //
    // UBlackboardComponent* BlackboardComp = CachedOwnerComp->GetBlackboardComponent();
    //
    // if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Charge))
    // {
    //     ExecuteTask(*CachedOwnerComp, nullptr); // Replay the charge attack if the state is still Charge
    // }
    // else
    // {
    //     FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded); // Finish the task successfully if the state has changed
    // }

    if (CachedOwnerComp)
    {
        // AMyEnemy* MyEnemyRef = Cast<AMyEnemy>(CharacterRef);
        // if (MyEnemyRef)
        // {
        //     // Trigger the stop of the sword trace event
        //     MyEnemyRef->MySwordTraceStopEvent();
        // }

        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}