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
	
	sceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	sceneRoot->SetupAttachment(RootComponent);

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	collisionComponent->SetupAttachment(sceneRoot);
	
	ballMeshPreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshPreview"));
	ballMeshPreview->SetupAttachment(sceneRoot);
	
	containerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	containerMesh->SetupAttachment(sceneRoot);
	
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABallContainer::OnBoxOverlap);
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

void ABallContainer::ReleaseBalls(float releaseSpeed)
{
	for (ABall* ball : storedBalls)
	{
		if (!IsValid(ball))
		{
			UE_LOG(LogTemp, Error, TEXT("ABallContainer::ReleaseBalls : invalid Ball"))
			continue;
		}
		
		ball->ReleaseFromStationary(releaseSpeed);
	}
}

void ABallContainer::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ABallContainer %s : Collision detected with %s"), *GetName(), *OtherActor->GetName());

	ABall* collidedBall = Cast<ABall>(OtherActor);

	if (!IsValid(collidedBall))
	{
		return;
	}
	
	collidedBall->SetStationaryAtLocation(ballMeshPreview->GetComponentLocation());
	storedBalls.Add(collidedBall);
	
	OutputSignal();
}

