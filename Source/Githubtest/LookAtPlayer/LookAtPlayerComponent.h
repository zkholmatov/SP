// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookAtPlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GITHUBTEST_API ULookAtPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

	// Speed at which the actor rotates to face the player
	UPROPERTY(EditAnywhere)
	float speed {1000.0f};

public:	
	// Sets default values for this component's properties
	ULookAtPlayerComponent();

	// Flag to determine if the component should rotate the actor
	UPROPERTY(VisibleAnywhere)
	bool bCanRotate = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
