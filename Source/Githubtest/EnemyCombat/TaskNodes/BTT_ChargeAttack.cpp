


#include "BTT_ChargeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Githubtest/Enemy/EnumEnemyState.h"


UBTT_ChargeAttack::UBTT_ChargeAttack()
{
    NodeName = "Charge Attack";
    bNotifyTick = false; // Disable tick notification
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    ChargeMontage = nullptr;

    AttackCounter = 0;
    MaxAttacks = FMath::RandRange(3, 5);
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }

    // Get the Blackboard component
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), false);

    // Verify the state is correctly set to Charge
    if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Charge))
    {
        // Ensure we have a valid animation montage
        if (ChargeMontage)
        {
            // Play the charge attack montage
            ControllerRef = OwnerComp.GetAIOwner();
            CharacterRef->PlayAnimMontage(ChargeMontage);
            AttackCounter++;
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Attack Counter: %d"), AttackCounter));
            }
            
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
    if (CachedOwnerComp)
    {
        UBlackboardComponent* BlackboardComp = CachedOwnerComp->GetBlackboardComponent();
        // BlackboardComp->SetValueAsBool(TEXT("TaskNodeCompleted"), true);
        if (BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Stunned))
        {
            AttackCounter = 0;
        }
        else if (AttackCounter >= MaxAttacks )
        {
            BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EnumEnemyState::Retreat));
            AttackCounter = 0;  
        }
        
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}



