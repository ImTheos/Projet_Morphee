// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Attractable.h"

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

	if (!isBeingAttracted)
	{
		return;
	}

	// add attracting logic, displacement towards attracting source
}

void AAttractable::SetNewAttractionSource(const AActor* newAttractionSource)
{
	attractionSource = newAttractionSource;
	isBeingAttracted = true;
}

bool AAttractable::IsBeingAttracted() const
{
	return isBeingAttracted;
}

