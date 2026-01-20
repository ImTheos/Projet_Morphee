// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/BallOwnerComponent.h"

#include "GameLogic/Ball/Ball.h"
#include "GameLogic/GameplayComponents/DetonateComponent.h"
#include "GameLogic/GameplayComponents/MagnetComponent.h"

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

	isLimitingBallDistance = limitBallDistanceOnSpawn;
	if (limitBallDistanceOnSpawn)
	{
		currentMaxBallDistance = maxDistanceOnSpawn;
	}
}


// Called every frame
void UBallOwnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isLimitingBallDistance)
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

	if (!IsBallTooFar(ownerActorLocation))
	{
		return;
	}
	
	// get the ball back
	ownedBall->SetActorLocation(ownerActorLocation);

	// TODO : link with the ball grabbing system when it's done
	// manual set of speed is here to keep things acceptable for now
	auto* magnetComponent = ballOwnerActor->GetComponentByClass<UMagnetComponent>();

	if (!IsValid(magnetComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("ABallOwnerComponent : Invalid MagnetComponent for actor"))
		return;
	}

	magnetComponent->GrabAttractedObject();
}

void UBallOwnerComponent::AssignBall(ABall* ball)
{
	ownedBall = ball;
	
	// DetonateComponent
	UDetonateComponent* detonateComponent = GetOwner()->GetComponentByClass<UDetonateComponent>();
	if (IsValid(detonateComponent))
	{
		detonateComponent->AssignBall(ball);
	}
	
	UMagnetComponent* magnetComponent = GetOwner()->GetComponentByClass<UMagnetComponent>();
	if (IsValid(magnetComponent))
	{
		magnetComponent->AssignBall(ball);
	}
}

void UBallOwnerComponent::EnableBallDistanceLimit(const float distanceLimit)
{
	currentMaxBallDistance	= distanceLimit;

	isLimitingBallDistance = true;
}

void UBallOwnerComponent::DisableBallDistanceLimit()
{
	isLimitingBallDistance = false;
}

bool UBallOwnerComponent::IsBallTooFar(const FVector& ownerActorLocation) const
{
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("BallOwnerComponent : ownedBall is not valid"));
		return false;
	}

	// Use of square to reduce compute time
	return FVector::DistSquared(ownedBall->GetActorLocation(), ownerActorLocation) > FMath::Square(currentMaxBallDistance);
}

