// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogic/GameplayComponents/DetonateComponent.h"

#include "GameLogic/Ball/Ball.h"
#include "GameLogic/Ball/BallEffects/BallEffect.h"

// Sets default values for this component's properties
UDetonateComponent::UDetonateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDetonateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDetonateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	remainingDetonationTime = FMath::Max(0.f, remainingDetonationTime - DeltaTime);
}

void UDetonateComponent::AssignBall(ABall* ball)
{
	ownedBall = ball;
}

void UDetonateComponent::Detonate()
{
	if (remainingDetonationTime > 0.f)
	{
		return;
	}
	
	remainingDetonationTime = detonationCooldown;
	
	// Add a base Detonate Logic here if needed
	
	// -- end of base logic
	
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UDetonateComponent::Detonate : Invalid ownedBall"));
		return;
	}
	
	if (ownedBall->GetBallState() != Free)
	{
		return;
	}
	
	ABallEffect* ballEffectInstance = ownedBall->GetBallEffectInstance();
	
	if (!IsValid(ballEffectInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDetonateComponent::Detonate : the ballEffectInstance is invalid"));
		return;
	}
	
	ballEffectInstance->Detonate();
}

