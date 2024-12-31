// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_CPP.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAIController_CPP::AAIController_CPP(const FObjectInitializer& ObjectInitializer)
{
	bStartAILogicOnPossess = true;
	PossessedPawn = nullptr;
	BehaviorTree = nullptr;
	BlackboardComponent = nullptr;

	// Create default components for the AI Controller
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	CppPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	// THIS IS A CUSTOM METHOD
	InitPerceptionComponent(); // Initialize perception component configs
}

void AAIController_CPP::BeginPlay()
{
	Super::BeginPlay();

	CppPerceptionComponent->SetSenseEnabled(*SightSenseConfig->GetSenseImplementation(), true);
}


void AAIController_CPP::InitPerceptionComponent()
{
	// Create default components for the AI Controller
	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightSenseConfig)
	{
		SightSenseConfig->SightRadius = sightRadius;
		SightSenseConfig->LoseSightRadius = loseSightRadius;
		SightSenseConfig->PeripheralVisionAngleDegrees = peripheralVisionAngleDeg;
		SightSenseConfig->SetMaxAge(maxAge);
		SightSenseConfig->AutoSuccessRangeFromLastSeenLocation = loseSightRadius - 10.f;
		SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = false;

		// Add new sight sense config to the perception component so it can be used as valid config
		CppPerceptionComponent->ConfigureSense(*SightSenseConfig);
		// Set the perception component to use sight as it's dominant sense
		// If GetSense implementation doesn't work for aisense object param use UAISense_Sight::StaticClass() or *SightSenseConfig->GetSenseImplementation()
		CppPerceptionComponent->SetDominantSense(DominantSense);
		// Binds perception update to perception component delegate class function.
		// In simple terms when the the class perception component gets an update via sight or damage it will call this function
		// PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnPerceptionUpdated);
		CppPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_CPP::OnTargetPerceptionUpdated);
	}

	// Create default components for the AI Controller
	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense"));
	// Does not need anything other than initialization because is event based 
	if(DamageSenseConfig)
	{
		// Add new sight sense config to the perception component so it can be used as valid config
		CppPerceptionComponent->ConfigureSense(*DamageSenseConfig);
	}
}

void AAIController_CPP::OnTargetPerceptionUpdated(AActor* PlayerActor, FAIStimulus const Stimulus)
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
	uint8 ENUMMERS = GetCurrentState();
	GetCurrentState();

	// if (ENUMMERS == EnumEnemyState::IDLE || ENUMMERS == EnumEnemyState::CHASE || ENUMMERS == EnumEnemyState::RANGE)
	// {
	// 	// playerActor->DisplayHealthBar();
	// 	SetStateAsIdle();
	// }
}

void AAIController_CPP::HandleLostSense(AActor* PlayerActor)
{
	uint8 ENUMMERS = GetCurrentState();
	GetCurrentState();

	// if (ENUMMERS == EnumEnemyState::IDLE || ENUMMERS == EnumEnemyState::CHASE || ENUMMERS == EnumEnemyState::RANGE)
	// {
	// 	// playerActor->DisplayHealthBar();
	// 	SetStateAsChase();
	// }
}

// This is automatically called by the engine with the owner is spawned and this method is then immediately invoked
void AAIController_CPP::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPawn = InPawn; // Set PossessedPawn to reference to pawn that has instance of AI controller ie InPawn

	// Check if the possessed pawn and the behavior tree are valid
	if(PossessedPawn && BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree); // If both are valid run behavior tree
	}
	
	SetStateAsIdle(); // Call Set idle to initialize the state of the possessed pawn 
	//
	//
	//
	//
	//
	//
	//
	
	// Still needs to implement the get ideal range as well as the set timer by event 
}

uint8 AAIController_CPP::GetCurrentState()
{
	if(BlackboardComponent)
	{
		return BlackboardComponent->GetValueAsEnum(TEXT("State"));
	}
	
	return BlackboardComponent->GetValueAsEnum(TEXT("State")); // Comment me out, for dev
	// return EnumEnemyState::IDLE;
}

void AAIController_CPP::SetStateAsIdle()
{
	if(BlackboardComponent)
	{
		// BlackboardComponent->SetValueAsEnum(TEXT("State"), static_cast<int8>(EnumEnemyState::IDLE));
	}
}

void AAIController_CPP::SetStateAsChase()
{
	if(BlackboardComponent)
	{
		// BlackboardComponent->SetValueAsEnum(TEXT("State"), static_cast<int8>(EnumEnemyState::Chase);
	}
}

// void AAIController_CPP::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
// {
// 	// Note from unreal doc page
// 	// Might want to move these to special "BP_AIPerceptionComponent". what ever that means 
// }

// void AAIController_CPP::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
// {
// 	Super::ActorsPerceptionUpdated(UpdatedActors);
// }

// bool AAIController_CPP::CanSenseActor(AActor* SensedActor, FName SenseName, const FAISenseID& StimulusType)
// {
// 	if (SensedActor == PossessedPawn)
// 	{
// 		FActorPerceptionBlueprintInfo PerceptionInfo;
// 		CppPerceptionComponent->GetActorsPerception(SensedActor, PerceptionInfo);
// 		if(StimulusType == UAISense::GetSenseID<UAISense_Sight>())
// 		{
// 			// Do Something if there is specific reason to do anything with different stimuli
// 			return true;
// 		}
// 	}
// }


