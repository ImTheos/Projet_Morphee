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
	
	// add base Detonate Logic here if needed (only effect override for now)
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UDetonateComponent::Detonate : Invalid ownedBall"));
		return;
	}
	
	TSubclassOf<UBallEffect> ballEffect = ownedBall->ballEffect;
	
	if (!IsValid(ballEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDetonateComponent::Detonate : the ballEffect is invalid"));
		return;
	}
		
	UBallEffect* defaultObject = Cast<UBallEffect>(ballEffect.Get()->GetDefaultObject());
		
	if (!IsValid(defaultObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("The default object is NULL"));
		return;
	}
		
	defaultObject->Detonate(ownedBall);
}

