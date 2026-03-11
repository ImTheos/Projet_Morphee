// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/BallOwnerComponent.h"

#include "GameLogic/Ball/Ball.h"
#include "GameLogic/GameplayComponents/DetonateComponent.h"
#include "GameLogic/GameplayComponents/MagnetComponent.h"
#include "GameLogic/Puzzle/BallContainer.h"

UBallOwnerComponent::UBallOwnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallOwnerComponent::BeginPlay()
{
	Super::BeginPlay();

	isLimitingBallDistance = limitBallDistanceOnSpawn;
	if (limitBallDistanceOnSpawn)
	{
		currentMaxBallDistance = maxDistanceOnSpawn;
	}
}

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
	ownedBall->speed = ownedBall->minimumSpeed;

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
	
	if (ownedBall->GetBallState() == Stationary)
	{
		ABallContainer* ballContainer = 
				Cast<ABallContainer>(
				Cast<UStaticMeshComponent>(ownedBall->influenceSource)->GetAttachParentActor()
				);
		
		if (IsValid(ballContainer) && ballContainer->ignoreBallDistanceLimit)
		{
			return false;
		}
	}

	// Use of square to reduce compute time
	return FVector::DistSquared(ownedBall->GetActorLocation(), ownerActorLocation) > FMath::Square(currentMaxBallDistance);
}

