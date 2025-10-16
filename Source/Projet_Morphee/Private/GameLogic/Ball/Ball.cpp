// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/Ball.h"

#include "Components/SphereComponent.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::TickAttract()
{
	if (!IsValid(attractionSource))
	{
		UE_LOG(LogTemp, Warning, TEXT("The attraction source is invalid"));
		return;
	}

	FVector newForwardVector = attractionSource->GetActorLocation() - GetActorLocation();
	SetActorRotation(newForwardVector.ToOrientationRotator());
}

void ABall::TickGrab()
{
	float distanceToAttractionSouce = FVector::Dist(GetActorLocation(), attractionSource->GetActorLocation());
	FVector newForwardVector;

	// TODO : add smoother transition between the three cases if needed
	if (distanceToAttractionSouce > grabAnimDistance + epsilonDistance * ballSpeed)
	{
		// Get the ball closer
		newForwardVector = attractionSource->GetActorLocation() - GetActorLocation();
	}
	else if (distanceToAttractionSouce < grabAnimDistance - epsilonDistance * ballSpeed)
	{
		// Get the ball further
		newForwardVector = GetActorLocation() - attractionSource->GetActorLocation();
	}
	else
	{
		// Rotate ball clockwise
		newForwardVector = FVector::CrossProduct(FVector::UpVector, attractionSource->GetActorLocation() - GetActorLocation());
	}

	SetActorRotation(newForwardVector.ToOrientationRotator());
}



