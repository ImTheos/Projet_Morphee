// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Attractable.h"

#include "Components/ShapeComponent.h"

// Sets default values
AAttractable::AAttractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAttractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (attractionState == Free)
	{
		return;
	}

	if (attractionState == Attracted)
	{
		TickAttract();
		return;
	}

	if (attractionState == Grabbed)
	{
		TickGrab();
		return;
	}

	// There should not be any logic here because all attractStates are used
	return;
}

void AAttractable::SetNewAttractionSource(const AActor* newAttractionSource)
{
	//SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = newAttractionSource;
	attractionState = Attracted;
}

void AAttractable::SetNewGrabSource(const AActor* newGrabSource)
{
	// TODO : fix collisions
	//SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	attractionSource = newGrabSource;
	attractionState = Grabbed;
}

EAttractableState AAttractable::GetAttractionState() const
{
	return attractionState;
}

void AAttractable::SetCollisionEnabled(ECollisionEnabled::Type collisionType) const
{
	auto* collisionComponent = GetComponentByClass<UShapeComponent>();

	if (!IsValid(collisionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AAttractable : No Collision Component found"))
		return;
	}

	collisionComponent->SetCollisionEnabled(collisionType);
}

void AAttractable::FreeFromAttraction()
{
	//SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = nullptr;
	attractionState = Free;
}

