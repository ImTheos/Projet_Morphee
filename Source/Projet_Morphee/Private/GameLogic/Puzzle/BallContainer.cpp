// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Puzzle/BallContainer.h"

#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "GameLogic/Ball/Ball.h"

// Sets default values
ABallContainer::ABallContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	BallMeshPreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshPreview"));

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABallContainer::OnBoxOverlap);
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

void ABallContainer::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ABallContainer %s : Collision detected with %s"), *GetName(), *OtherActor->GetName());

	ABall* CollidedBall = Cast<ABall>(OtherActor);

	if (!IsValid(CollidedBall))
	{
		return;
	}

	CollidedBall->speed = 0;
	CollidedBall->SetActorTransform(BallMeshPreview->GetComponentTransform());
}

