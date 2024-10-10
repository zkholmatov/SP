// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Retreat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Githubtest/EnumEnemyState.h"
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
    // Cache owner and character for further usage
    CachedOwnerComp = &OwnerComp;
    CharacterRef = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (!CharacterRef)
    {
        return EBTNodeResult::Failed;
    }

    // Get the Blackboard component
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // Verify the state is correctly set to Range
    if (BlackboardComp && BlackboardComp->GetValueAsEnum(TEXT("CurrentState")) == static_cast<uint8>(EnumEnemyState::Retreat))
    {
        // Ensure we have a valid animation montage
        if (RetreatMontage)
        {
            // Play the range attack montage
            ControllerRef = OwnerComp.GetAIOwner();
            CharacterRef->PlayAnimMontage(RetreatMontage);

            LookAtPlayer();

            // Set up a timer to complete the task after the animation duration
            CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(
                RetreatTimerHandle, [this]()
                {
                    FinishRetreatTask();
                },
                RetreatMontage->GetPlayLength(), false);

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTT_Retreat::FinishRetreatTask()
{
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTT_Retreat::LookAtPlayer()
{
    // Continuously turn towards the player
    FVector PlayerLocation = CharacterRef->GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterRef->GetActorLocation(), PlayerLocation);
    CharacterRef->SetActorRotation(LookAtRotation);
}
