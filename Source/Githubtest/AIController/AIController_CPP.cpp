// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController_CPP.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Githubtest/Enemy/EnumEnemyState.h"

AAIController_CPP::AAIController_CPP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// bStartAILogicOnPossess = true;
	// PossessedPawn = nullptr;

	sightRadius = 1000.0f;
	loseSightRadius = 1200.0f;
	peripheralVisionAngleDeg = 90.0f;
	maxAge = 5.0f;

	// Create default components for the AI Controller
	CppPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("CppPerceptionComponent"));
	SetPerceptionComponent(*CppPerceptionComponent);
	
	// SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config")); // this sometimes gets destroyed in garbage collection
	SightSenseConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("Sight Config")); // no garbage for me 
	if (!SightSenseConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SightSenseConfig in constructor."));
		// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Failed to create SightSenseConfig in constructor."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("created SightSenseConfig in constructor."));
		// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("created SightSenseConfig in constructor."));
	}

	// Create default senses for the AI Controller perception component
	// Add new sight sense config to the perception component so it can be used as valid config
	// Set the perception component to use sight as it's dominant sense
	// If GetSense implementation doesn't work for AI sense object param use UAISense_Sight::StaticClass() or *SightSenseConfig->GetSenseImplementation()
	// CppPerceptionComponent->SetDominantSense(DominantSense); // Didnt work idk why
	CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
	CppPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	// Add new sight sense config to the perception component so it can be used as valid config
	// DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense")); // this sometimes gets destroyed in garbage collection
	DamageSenseConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Damage>(this,TEXT("DamageSense")); // no garbage for me 
	CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);
}

void AAIController_CPP::BeginPlay()
{
	Super::BeginPlay();

	// THIS IS A CUSTOM METHOD
	InitPerceptionConfig();// Initialize perception component configs

	if (CppPerceptionComponent)
	{
		// Binds perception update to perception component delegate class function.
		// In simple terms when the the class perception component gets an update via sight or damage it will call this function
		CppPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PerceptionComponent in CPP is null in BeginPlay."));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("PerceptionComponent in CPP is null in BeginPlay."));
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("I BEGAN"));
	BeginPlayExtended();
}

// Used to initialize and wrap up all the config for the perception senses
void AAIController_CPP::InitPerceptionConfig()
{
	// Check to see if stuff is getting destroyed in garbage collection, which only happens sometimes? idk
	if (!CppPerceptionComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("CppPerceptionComponent is null in InitPerceptionConfig."));
		return;
	}
	if (!SightSenseConfig)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("SightSenseConfig is null in InitPerceptionConfig."));
		return;
	}
	
	// Create default config for the sight sense and damage sense
	if(SightSenseConfig && CppPerceptionComponent)
	{
		SightSenseConfig->SightRadius = sightRadius;
		SightSenseConfig->LoseSightRadius = loseSightRadius;
		SightSenseConfig->PeripheralVisionAngleDegrees = peripheralVisionAngleDeg;
		SightSenseConfig->SetMaxAge(maxAge);
		// SightSenseConfig->AutoSuccessRangeFromLastSeenLocation = loseSightRadius - 10.f;
		SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
		CppPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SightSenseConfig && CppPerceptionComponent are bad"));
	}
	// Does not need anything other than initialization because is event based 
	if(DamageSenseConfig && CppPerceptionComponent)
	{
		CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);
	}
}

void AAIController_CPP::OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus Stimulus)
{
	// Check if the actor is the player character
	// Stimulus.WasSuccessfullySensed() is essentially doing what can sense actors functionality was doing in BP
	if (Stimulus.WasSuccessfullySensed())
	{
		HandleSensed(PlayerActor);
	}
	else
	{
		// HandleLostSense(PlayerActor);
		// Check how long the stimulus has been inactive
		if (Stimulus.GetAge() >= maxAge) // Wait for 1 second before triggering "lost" sense
		{
			HandleLostSense();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Looking for lost player... where are you, nerd? Stimulus Age: %f"),
			Stimulus.GetAge() ));
		}
		// this shit is a nightmare in C++ here is the link https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-timers-in-unreal-engine
		GetWorld()->GetTimerManager().SetTimer(
				ForgetPlayerTimerHandle, 
				this, 
				&AAIController_CPP::HandleLostSense, 
				maxAge,  // Delay for the duration of MaxAge
				false
			);
	}
	
	
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("targeting"));
}

void AAIController_CPP::HandleSensed(AActor* PlayerActor)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ForgetPlayerTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ForgetPlayerTimerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ForgetPlayer timer cleared because actor was sensed again."));
	}
	
	if (PlayerActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		uint8 ENUMMERS = GetCurrentState();
	
		// if (ENUMMERS == EnumEnemyState::Idle || ENUMMERS == EnumEnemyState::Chase || ENUMMERS == EnumEnemyState::Range)
		// {
		//  This stuff below was a from a forum :)
		
			// if(UFunction* TriggerFunction = PossessedPawn->FindFunction(TEXT("DisplayHealthBar")))
			// {
			// 	// Create a buffer just in case (if we send a null buffer, the system will crash if the event has parameters).
			// 	// (Check the codebase to see examples sending params.)
			// 	uint8* ParamsBuffer = static_cast<uint8*>(FMemory_Alloca(TriggerFunction->ParmsSize));
			// 	FMemory::Memzero(ParamsBuffer, TriggerFunction->ParmsSize);
			// 	PlayerActor->ProcessEvent(TriggerFunction, ParamsBuffer);
			// }
		
			SetStateAsChase();
		}
	// }
	FoundActor = PlayerActor;
	OnActorFound(PlayerActor); // This a function extension for blueprints event graph 
}

void AAIController_CPP::HandleLostSense()
{
	if (FoundActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		uint8 ENUMMERS = GetCurrentState();
		SetStateAsIdle();
	}

	OnActorLost(FoundActor); // This a function extension for blueprints event graph 
}

UBlackboardComponent* AAIController_CPP::GetBlackboardComp()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	
	if (!BlackboardComp) 
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComp is not valid!"));
		return nullptr; 
	}

	return BlackboardComp; 
}

uint8 AAIController_CPP::GetCurrentState()
{
	if(UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		return BlackboardComp->GetValueAsEnum(TEXT("State"));
	}
	
	return EnumEnemyState::Idle;
}

void AAIController_CPP::SetStateAsIdle()
{
	if(UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("State"), static_cast<int8>(EnumEnemyState::Idle));
	}
}

void AAIController_CPP::SetStateAsChase()
{
	if(UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(TEXT("State"), static_cast<int8>(EnumEnemyState::Chase));
	}
}