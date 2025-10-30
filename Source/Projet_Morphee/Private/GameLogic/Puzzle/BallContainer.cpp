// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Puzzle/BallContainer.h"

#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"

// Sets default values
ABallContainer::ABallContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));

}

// Called when the game starts or when spawned
void ABallContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

