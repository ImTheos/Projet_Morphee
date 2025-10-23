// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/BallOwnerComponent.h"

#include "GameLogic/Ball/Ball.h"
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
	// manual set of ballSpeed is here to keep things acceptable for now
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


void UBallOwnerComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	UE_LOG(LogTemp, Log, TEXT("BallOwnerComponent : Serialize called"));
}

void UBallOwnerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UE_LOG(LogTemp, Log, TEXT("BallOwnerComponent : InitializeComponent called"))
}

#if WITH_EDITOR

void UBallOwnerComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UBallOwnerComponent, ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("ownedBall change has been detected"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("no ownedball change"));
	}
}
#endif

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

