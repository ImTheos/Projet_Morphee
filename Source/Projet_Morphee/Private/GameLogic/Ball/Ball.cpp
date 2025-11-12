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

	if (ballState == Attracted)
	{
		TickAttract();
		return;
	}

	if (ballState == Grabbed)
	{
		TickGrab();
		return;
	}
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
	if (distanceToAttractionSouce > grabAnimDistance + epsilonDistance)
	{
		// Get the ball closer
		newForwardVector = attractionSource->GetActorLocation() - GetActorLocation();
	}
	else if (distanceToAttractionSouce < grabAnimDistance - epsilonDistance)
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

void ABall::SetNewAttractionSource(const AActor* newAttractionSource)
{
	SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = newAttractionSource;
	ballState = Attracted;
}

void ABall::SetNewGrabSource(const AActor* newGrabSource)
{
	// TODO : fix collisions 
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	attractionSource = newGrabSource;
	ballState = Grabbed; 
}

EBallState ABall::GetBallState() const
{
	return ballState;
}

void ABall::SetCollisionEnabled(ECollisionEnabled::Type collisionType) const
{
	auto* collisionComponent = GetComponentByClass<UShapeComponent>();

	if (!IsValid(collisionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall : No Collision Component found"))
		return;
	}

	collisionComponent->SetCollisionEnabled(collisionType);
}

void ABall::FreeFromAttraction()
{
	SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = nullptr;
	ballState = Free;
}

void ABall::SetStationaryAtLocation(const FVector& location)
{
	SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	attractionSource = nullptr;
	ballState = Stationary;
	speed = 0;
	
	// TODO : Add animation for ball transition to new location
	SetActorLocation(location);
}



