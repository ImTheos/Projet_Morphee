// Fill out your copyright notice in the Description page of Project Settings.
#include "GameLogic/GameplayComponents/MagnetComponent.h"

#include "GameLogic/Ball/Ball.h"
#include "GameLogic/Puzzle/BallContainer.h"

UMagnetComponent::UMagnetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMagnetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	remainingCooldown = FMath::Max(remainingCooldown - DeltaTime, 0.f);
	
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
		InitMagnetCooldown();
		GrabAttractedObject();
	}
}

void UMagnetComponent::AssignBall(ABall* ball)
{
	ownedBall = ball;
}

void UMagnetComponent::AttractObject()
{
	AActor* componentOwner = GetOwner();

	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : the component owner is not valid (this should not happen)"))
	}
	
	ownedBall->SetBallState(Attracted, componentOwner);
}

void UMagnetComponent::GrabAttractedObject()
{
	AActor* componentOwner = GetOwner();
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

void UMagnetComponent::InitMagnetCooldown()
{
	remainingCooldown = cooldown;
}

void UMagnetComponent::ActivateMagnet()
{
	if (remainingCooldown > 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Magnet in cooldown"));
		UE_LOG(LogTemp, Display, TEXT("UMagnetComponent : Magnet still in cooldown"));
		return;
	}
	
	if (!IsValid(ownedBall))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No ownedBall has been set"));
		return;
	}
	
	isMagnetActive = true;

	// If object is already being attracted, no need to set up attraction again
	if (ownedBall->GetBallState() == Grabbed || ownedBall->GetBallState() == Attracted)
	{
		return;
	}
	
	if (ownedBall->GetBallState() == Stationary)
	{
		ABallContainer* ballContainer = 
			Cast<ABallContainer>(
			Cast<UStaticMeshComponent>(ownedBall->influenceSource)->GetAttachParentActor()
			);
		
		if (!IsValid(ballContainer))
		{
			// to avoid blocking the game, but not supposed to happen
			AttractObject();
			ownedBall->speed = ownedBall->minimumSpeed;
			return;
		}
		
		if (!ballContainer->canReleaseFromMagnet)
		{
			return;
		}
		
		ballContainer->BallReleased(ownedBall);
		AttractObject();
	}
	
	if (ownedBall->GetBallState() == Free)
	{
		AttractObject();
	}
	
	ownedBall->speed = FMath::Max(ownedBall->minimumSpeed, ownedBall->speed);
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

