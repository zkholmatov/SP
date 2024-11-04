


#include "BTT_StandardAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Githubtest/Enemy/EnumEnemyState.h"


UBTT_StandardAttack::UBTT_StandardAttack()
{
    NodeName = "Attack";
    bNotifyTick = false; // Disable tick notification
    bCreateNodeInstance = true;

    ControllerRef = nullptr;
    CharacterRef = nullptr;
    CachedOwnerComp = nullptr;
    AttackMontage = nullptr;
}

EBTNodeResult::Type UBTT_StandardAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }
    
    // Ensure we have a valid animation montage
    if (AttackMontage)
    {
        // Play the charge attack montage
        ControllerRef = OwnerComp.GetAIOwner();
        CharacterRef->PlayAnimMontage(AttackMontage);

        // Set up a timer to complete the task after the animation duration
        CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle, [this]()
        {
            FinishAttackTask();
        },
        AttackMontage->GetPlayLength(), false);
        
        return EBTNodeResult::InProgress;
    }
    
    return EBTNodeResult::Failed;
}

void UBTT_StandardAttack::FinishAttackTask()
{
    
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
    else if (GEngine)
    {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CachedOwnerComp is null"));
    }
}