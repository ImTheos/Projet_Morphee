// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/MagnetComponent.h"

#include "GameLogic/Ball/Ball.h"

// Sets default values for this component's properties
UMagnetComponent::UMagnetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMagnetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMagnetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No ownedBall has been set"));
		return;
	}

	// get owner
	const AActor* componentOwner = GetOwner();

	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("MagnetComponent : the component owner is not valid (this should not happen)"))
	}

	// test distance between owner and magneted object
	if (FVector::Dist(componentOwner->GetActorLocation(), ownedBall->GetActorLocation()) > magnetRadius)
	{
		return;
	}

	if (!isMagnetActive && ownedBall->GetBallState() == Attracted)
	{
		ownedBall->SetBallState(Free);
	}
	
	// grab object
	if (isMagnetActive)
	{
		GrabAttractedObject();
	}
}

void UMagnetComponent::AssignBall(ABall* ball)
{
	ownedBall = ball;
}

void UMagnetComponent::AttractObject()
{
	const AActor* componentOwner = GetOwner();

	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : the component owner is not valid (this should not happen)"))
	}
	
	ownedBall->SetBallState(Attracted, componentOwner);
}

void UMagnetComponent::GrabAttractedObject()
{
	const AActor* componentOwner = GetOwner();
	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : the component owner is not valid (this should not happen)"))
	}

	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No ownedBall has been set"));
		return;
	}

	ownedBall->SetBallState(Grabbed, componentOwner);
}

void UMagnetComponent::ActivateMagnet(float minimumSpeed)
{
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No ownedBall has been set"));
		return;
	}
	
	isMagnetActive = true;

	// If object is already being attracted, no need to set up attraction again
	if (ownedBall->GetBallState() != Free)
	{
		return;
	}

	ownedBall->speed = FMath::Max(minimumSpeed, ownedBall->speed);

	AttractObject();
}

void UMagnetComponent::DeactivateMagnet()
{
	isMagnetActive = false;
}

void UMagnetComponent::FreeAttractedObject()
{
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No ownedBall has been set"));
		return;
	}

	ownedBall->SetBallState(Free);
}

