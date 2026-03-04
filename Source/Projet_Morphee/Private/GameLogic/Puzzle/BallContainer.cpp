// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Puzzle/BallContainer.h"

#include "Components/BoxComponent.h"
#include "GameLogic/Ball/Ball.h"

ABallContainer::ABallContainer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	sceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	sceneRoot->SetupAttachment(RootComponent);

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	collisionComponent->SetupAttachment(sceneRoot);
	
	ballMeshPreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshPreview"));
	ballMeshPreview->SetupAttachment(sceneRoot);
	
	containerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	containerMesh->SetupAttachment(sceneRoot);
}

void ABallContainer::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(collisionComponent))
	{
		return;
	}
	
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABallContainer::OnBoxBeginOverlap);
	collisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABallContainer::OnBoxEndOverlap);
}

void ABallContainer::BallReleased(ABall* releasedBall)
{
	storedBalls.Remove(releasedBall);
	releasedBalls.Add(releasedBall);
	
	containerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, releasedBall]()
	{
		containerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		releasedBalls.Remove(releasedBall);
	});
	
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		return;	
	}
	
	world->GetTimerManager().SetTimer(timerHandle, timerDelegate, 1.0f, false);
	
	BallReleasedSignal();
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
	
	BallReleasedSignal() ;
}

void ABallContainer::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABall* collidedBall = Cast<ABall>(OtherActor);

	if (!IsValid(collidedBall) || collidedBall->GetBallState() == Grabbed || collidedBall->GetBallState() == Stationary)
	{
		return;
	}
	
	collidedBall->SetBallState(Stationary, ballMeshPreview);
	storedBalls.Add(collidedBall);
	
	BallReceivedSignal();
}

void ABallContainer::TestFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("TestFunction"));
}

void ABallContainer::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherOverlappedComponent, int OtherBodyIndex)
{
	ABall* collidedBall = Cast<ABall>(OtherActor);
	
	if (!IsValid(collidedBall))
	{
		return;
	}
	
	if (releasedBalls.Contains(collidedBall))
	{
		releasedBalls.Remove(collidedBall);
		containerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

