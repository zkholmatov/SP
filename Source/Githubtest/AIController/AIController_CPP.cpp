// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_CPP.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Githubtest/Enemy/EnumEnemyState.h"

AAIController_CPP::AAIController_CPP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bStartAILogicOnPossess = true;
	PossessedPawn = nullptr;

	sightRadius = 1000.0f;
	loseSightRadius = 1200.0f;
	peripheralVisionAngleDeg = 90.0f;
	maxAge = 5.0f;

	// Create default components for the AI Controller
	// BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	CppPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*CppPerceptionComponent);

	// Create default senses for the AI Controller perception component
	
	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	// Add new sight sense config to the perception component so it can be used as valid config
	CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
	// Set the perception component to use sight as it's dominant sense
	// If GetSense implementation doesn't work for AI sense object param use UAISense_Sight::StaticClass() or *SightSenseConfig->GetSenseImplementation()
	// CppPerceptionComponent->SetDominantSense(DominantSense); // Didnt work idk why
	CppPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	
	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense"));
	// Add new sight sense config to the perception component so it can be used as valid config
	CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);
}

void AAIController_CPP::BeginPlay()
{
	Super::BeginPlay();

	// THIS IS A CUSTOM METHOD
	InitPerceptionConfig();// Initialize perception component configs
	// CppPerceptionComponent->SetSenseEnabled(*SightSenseConfig->GetSenseImplementation(), true);
	CppPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);

	if (PerceptionComponent)
	{
		// Binds perception update to perception component delegate class function.
		// In simple terms when the the class perception component gets an update via sight or damage it will call this function
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PerceptionComponent in CPP is null in BeginPlay."));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("I BEGAN"));
	BeginPlayExtended();
}

// // This is automatically called by the engine with the owner is spawned and this method is then immediately invoked
// void AAIController_CPP::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("possessed"));
//
// 	PossessedPawn = InPawn; // Set PossessedPawn to reference to pawn that has instance of AI controller ie InPawn
//
// 	// Check if the possessed pawn and the behavior tree are valid
// 	if(BehaviorTree)
// 	{
// 		RunBehaviorTree(BehaviorTree);
// 		// if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
// 		// {
// 		// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("behavior happening"));
// 		// 	
// 		// }
// 		// else
// 		// {
// 		// 	UE_LOG(LogTemp, Error, TEXT("Failed to initialize the Blackboard Component!"));
// 		// }
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("pawn and or bt are bad "));
// 	}
// 	
// 	SetStateAsIdle(); // Call Set idle to initialize the state of the possessed pawn 
// 	OnPossessExtended(PossessedPawn);
// }

// Used to initialize and wrap up all the config for the perception senses
void AAIController_CPP::InitPerceptionConfig()
{
	// Create default components for the AI Controller
	if(SightSenseConfig && PerceptionComponent)
	{
		SightSenseConfig->SightRadius = sightRadius;
		SightSenseConfig->LoseSightRadius = loseSightRadius;
		SightSenseConfig->PeripheralVisionAngleDegrees = peripheralVisionAngleDeg;
		SightSenseConfig->SetMaxAge(maxAge);
		SightSenseConfig->AutoSuccessRangeFromLastSeenLocation = loseSightRadius - 10.f;
		SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = false;
		
		CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
		CppPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	}
	
	// Does not need anything other than initialization because is event based 
	if(DamageSenseConfig && PerceptionComponent)
	{
		CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);
	}

	

	// Binds perception update to perception component delegate class function.
	// In simple terms when the the class perception component gets an update via sight or damage it will call this function
	// CppPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnPerceptionUpdated);
	// CppPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnTargetPerceptionUpdated);

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Configured!"));
}

void AAIController_CPP::OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus const& Stimulus)
{
	// Check if the actor is the player character
	// Stimulus.WasSuccessfullySensed() is essentially doing what can sense actors functionality was doing in BP
	if (Stimulus.WasSuccessfullySensed() && PlayerActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		
		HandleSensed(PlayerActor);
	}
	else
	{
		HandleLostSense(PlayerActor);
	}
}

void AAIController_CPP::HandleSensed(AActor* PlayerActor)
{
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
	OnActorFound(PlayerActor); // This a function extension for blueprints event graph 
}

void AAIController_CPP::HandleLostSense(AActor* PlayerActor)
{
	if (PlayerActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		uint8 ENUMMERS = GetCurrentState();
		SetStateAsIdle();
	}

	OnActorLost(PlayerActor); // This a function extension for blueprints event graph 
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