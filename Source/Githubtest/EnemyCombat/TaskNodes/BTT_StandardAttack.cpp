


#include "BTT_StandardAttack.h"
#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

#include "Githubtest/Enemy/MyEnemy.h"


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
        // https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/AnimGraphRuntime/UPlayMontageCallbackProxy?application_version=5.3
        // https://forums.unrealengine.com/t/play-montage-in-c-with-onblendout-oninterrupted-etc/447184/2
        // Create a proxy object to play the montage attack montage
        // This proxy automatically tracks the montage and provides events for when it completes or gets interrupted
        // Internally queries interrupts or complete etc.
        //This is identical to using Play Montage in event graph
        UPlayMontageCallbackProxy* PlayMontageCallbackProxy =
            UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(CharacterRef->GetMesh(), AttackMontage, 1.0f);

        // Binds perception update to perception component delegate class function
        // In simple terms when the the class perception component gets an update via sight or damage it will call this function
        PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &UBTT_StandardAttack::HandleMontageCompleted);
        PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &UBTT_StandardAttack::HandleMontageInterrupted);
        
    /// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

       // this works but will pause wait after interrupt for duration of getplaylength()
        // Play the charge attack montage
        // ControllerRef = OwnerComp.GetAIOwner();
        // CharacterRef->PlayAnimMontage(AttackMontage);
        // // Set up a timer to complete the task after the animation duration
        // CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
        // AttackTimerHandle, [this]()
        // {
        //     FinishAttackTask();
        // },
        // AttackMontage->GetPlayLength(), false);
        
        return EBTNodeResult::InProgress;
    }
    
    return EBTNodeResult::Failed;
}


void UBTT_StandardAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Montage interrupted!"));
    }
    else
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Montage completed!"));
    }
}


EBTNodeResult::Type UBTT_StandardAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack Task Aborted"));
    if (ControllerRef && CharacterRef)
    {
        // Stop the character's movement
        ControllerRef->StopMovement();
    
        // Clear attack timers
        CachedOwnerComp->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    }
    
    // // EBTNodeResult::Aborted;
    // Super::FinishLatentAbort(*CachedOwnerComp);
    //
    // // // Call the base class implementation
    // // Super::AbortTask(OwnerComp, NodeMemory);
    
    return Super::AbortTask(OwnerComp, NodeMemory);
}


void UBTT_StandardAttack::FinishAttackTask()
{
    
    if (CachedOwnerComp)
    {
        CachedOwnerComp->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
    else if (GEngine)
    {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CachedOwnerComp is null"));
    }
}

void UBTT_StandardAttack::HandleMontageCompleted(FName NotifyName)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Montage completed "));
    if (UAnimInstance* AnimInstance = CharacterRef->GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Stop(0.2f);
    }
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}


void UBTT_StandardAttack::HandleMontageInterrupted(FName NotifyName)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Montage Interrupted"));
    if (UAnimInstance* AnimInstance = CharacterRef->GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Stop(0.2f);
    }
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
}
