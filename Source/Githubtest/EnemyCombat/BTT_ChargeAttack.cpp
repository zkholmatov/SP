


#include "BTT_ChargeAttack.h"
#include "AIController.h"
#include "Githubtest/MyEnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Githubtest/EnumEnemyState.h"
#include "Githubtest/MyEnemy.h"

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
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    
    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    
    if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Charge))
    {
        if (ChargeMontage)
        {
            ControllerRef = OwnerComp.GetAIOwner();
            float MontageDuration = CharacterRef->PlayAnimMontage(ChargeMontage);
            
            AMyEnemy* MyEnemyRef = Cast<AMyEnemy>(CharacterRef);
            if (MyEnemyRef)
            {
                MyEnemyRef->MySwordTraceEvent();
            }
            
            // Ensure the timer delegate is correctly set up to call FinishAttackTask
            FTimerDelegate TimerDel;
            TimerDel.BindUFunction(this, FName("FinishAttackTask"));
            
            // Set timer for animation duration
            ControllerRef->GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDel, MontageDuration, false);
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
        AMyEnemy* MyEnemyRef = Cast<AMyEnemy>(CharacterRef);
        if (MyEnemyRef)
        {
            // Trigger the stop of the sword trace event
            MyEnemyRef->MySwordTraceStopEvent();
        }

        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}