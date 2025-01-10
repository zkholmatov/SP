// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController_CPP.h"
#include "BehaviorTree/BehaviorTree.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Githubtest/Enemy/EnumEnemyState.h"

AAIController_CPP::AAIController_CPP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// bStartAILogicOnPossess = true;

	sightRadius = 1000.0f;
	loseSightRadius = 1200.0f;
	peripheralVisionAngleDeg = 90.0f;
	maxAge = 3.0f;

	// Create default components for the AI Controller
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/CoreUObject/UObject/FObjectInitializer
	CppPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("CppPerceptionComponent"));
	// CppPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("CppPerceptionComponent"));
	SetPerceptionComponent(*CppPerceptionComponent);
	
	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config")); // this sometimes gets destroyed in garbage collection
	// SightSenseConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("Sight Config")); // no garbage for me
	
	if (!SightSenseConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SightSenseConfig in constructor."));
		// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Failed to create SightSenseConfig in constructor."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("created SightSenseConfig in constructor."));
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
	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense")); // this sometimes gets destroyed in garbage collection
	// DamageSenseConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Damage>(this,TEXT("DamageSense")); // no garbage for me 
	CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);

	// Initialize all the config for the senses to be overwritten later, maybe if i can get it tot work lol
	InitPerceptionConfig();
}

void AAIController_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	if (CppPerceptionComponent)
	{
		// Binds perception update to perception component delegate class function
		// In simple terms when the the class perception component gets an update via sight or damage it will call this function
		CppPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PerceptionComponent in CPP is null in BeginPlay."));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("PerceptionComponent in CPP is null in BeginPlay."));
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CPP I BEGAN")); // For testing 
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

// Does not work :) which sucks 
// void AAIController_CPP::PostInitializeComponents()
// {
// 	Super::PostInitializeComponents();

	// InitializeAIConfigInEditor();
	//
	// // Ensure components exist before applying editor values
	// if (SightSenseConfig && CppPerceptionComponent)
	// {
	// 	// Apply editor-configured values to the SightSenseConfig
	// 	SightSenseConfig->SightRadius = sightRadius;
	// 	SightSenseConfig->LoseSightRadius = loseSightRadius;
	// 	SightSenseConfig->PeripheralVisionAngleDegrees = peripheralVisionAngleDeg;
	// 	SightSenseConfig->SetMaxAge(maxAge);
 //        
	// 	// Reapply the config to the perception component to reflect changes
	// 	CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
	//
	// 	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT(" POST SightSenseConfig updated with editor variables."));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("POST SightSenseConfig or CppPerceptionComponent is null."));
	// }
// }


void AAIController_CPP::OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus Stimulus)
{
	// Check if the actor is the player character
	// Stimulus.WasSuccessfullySensed() is essentially doing what can sense actors functionality was doing in BP
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
	{
		HandleOnDamage(PlayerActor);
	}
	else if (Stimulus.WasSuccessfullySensed())
	{
		HandleSensed(PlayerActor);
	}
	else
	{
		// HandleLostSense(PlayerActor);
		// Check how long the stimulus has been inactive
		if (Stimulus.GetAge() >= maxAge) // This is mainly an edge case 
		{
			HandleLostSense();
		}
		else
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Looking for lost player... where are you, nerd? Stimulus Age: %f"),
			// Stimulus.GetAge() ));
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
	
	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("targeting"));
}


void AAIController_CPP::HandleSensed(AActor* PlayerActor)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ForgetPlayerTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ForgetPlayerTimerHandle);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ForgetPlayer timer cleared because actor was sensed again."));
	}
	
	if (PlayerActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		SetStateAsChase();
	}
	
	FoundActor = PlayerActor;
	OnActorFoundCPP(PlayerActor); // This a function extension for blueprints event graph 
}


void AAIController_CPP::HandleLostSense()
{
	if (FoundActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		SetStateAsIdle();
	}

	OnActorLostCPP(FoundActor); // This a function extension for blueprints event graph 
}


void AAIController_CPP::HandleOnDamage(AActor* DamagingActor)
{
	if (DamagingActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		SetStateAsChase();
	}

	OnDamageSenseCPP(DamagingActor);
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