// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/BallOwnerComponent.h"

#include "GameLogic/Ball/Ball.h"

// Sets default values for this component's properties
UBallOwnerComponent::UBallOwnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBallOwnerComponent::BeginPlay()
{
	Super::BeginPlay();

	limitBallDistance = limitBallDistanceOnSpawn;
	maxBallDistance = defaultMaxBallDistance;
}


// Called every frame
void UBallOwnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!limitBallDistance)
	{
		return;
	}

	AActor* ballOwnerActor = GetOwner();

	if (!IsValid(ballOwnerActor))
	{
		// how tf could that happen
		UE_LOG(LogTemp, Error, TEXT("BallOwnerComponent : parent actor is invalid ?! (how can that happen ?)"))
	}

	FVector ownerActorLocation = ballOwnerActor->GetActorLocation();

	if (IsBallTooFar(ownerActorLocation))
	{
		// get the ball back
		ownedBall->SetActorLocation(ownerActorLocation);

		// TODO : link with the ball grabbing system when it's done
		// manual set of ballSpeed is here to keep things acceptable for now

		ownedBall->ballSpeed = 0;
	}
}

void UBallOwnerComponent::AssignBall(ABall* ball)
{
	ownedBall = ball;
}

void UBallOwnerComponent::EnableBallDistanceLimit(const float distanceLimit)
{
	maxBallDistance	= distanceLimit;

	limitBallDistance = true;
}

void UBallOwnerComponent::DisableBallDistanceLimit()
{
	limitBallDistance = false;
}

bool UBallOwnerComponent::IsBallTooFar(const FVector& ownerActorLocation) const
{
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("BallOwnerComponent : ownedBall is not valid"));
		return false;
	}

	// Use of square to reduce compute time
	return FVector::DistSquared(ownedBall->GetActorLocation(), ownerActorLocation) > FMath::Square(maxBallDistance);
}

