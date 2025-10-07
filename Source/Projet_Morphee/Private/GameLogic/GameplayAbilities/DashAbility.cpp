// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayAbilities/DashAbility.h"

#include "MyCPPCharacter.h"
#include "Components/CapsuleComponent.h"


bool UDashAbility::Init()
{
	AActor* playerActor = GetAvatarActorFromActorInfo();
	AMyCPPCharacter* playerCharacter = Cast<AMyCPPCharacter>(playerActor);

	if (!IsValid(playerCharacter))
	{
		return false;
	}

	UCapsuleComponent* playerCapsule = playerCharacter->GetCapsuleComponent();

	if (!playerCapsule)
	{
		return false;
	}

	capsuleRadius = playerCapsule->GetUnscaledCapsuleRadius();
	capsuleHalfHeight = playerCapsule->GetUnscaledCapsuleHalfHeight();

	return true;
}

FVector UDashAbility::CalculateDashTeleport(FVector actorLocation, FVector targetLocation)
{
	UWorld* world = GetWorld();

	float dashDistance = FVector::Dist(actorLocation, targetLocation);
	int capsuleAmount = 1 + dashDistance / capsuleRadius;
	FVector dashDirection = (targetLocation - actorLocation).GetSafeNormal() * dashDistance / capsuleAmount;

	FVector differenceThresholdVector = FVector(0.f, 0.f, teleportHeightDifferenceThreshold);
	FHitResult traceHitResult;
	FCollisionShape traceShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);

	FVector dashLocation = targetLocation;

	for (int i = 0; i < capsuleAmount; i++)
	{
		world->SweepSingleByChannel(traceHitResult,
								dashLocation + differenceThresholdVector,
								dashLocation - differenceThresholdVector,
								FQuat::Identity, dashTraceChannel, traceShape);

		float testDotProduct = traceHitResult.Normal.Dot(FVector::UpVector);
		
		if (traceHitResult.bBlockingHit && traceHitResult.PenetrationDepth == 0.f && !DashCollidesWithObstacle(actorLocation, traceHitResult.Location))
		{
			DrawDebugCapsule(world, traceHitResult.Location, capsuleHalfHeight, capsuleRadius, FQuat::Identity, FColor::Purple, false, 10);
			return traceHitResult.Location;
		}

		DrawDebugCapsule(world, traceHitResult.Location, capsuleHalfHeight/4, capsuleRadius/4, FQuat::Identity, FColor::Blue, false, 10);
		dashLocation -= dashDirection;
	}
	
	return actorLocation;
}

FVector UDashAbility::CalculateDashEndPostTeleport(FVector actorLocation, FVector targetLocation, FVector postTeleportLocation)
{
	float remainingDistance = dashMaxDistance - FVector::DistXY(actorLocation, postTeleportLocation);

	if (remainingDistance < capsuleRadius)
	{
		return postTeleportLocation;
	}

	int capsuleAmount = 1 + remainingDistance / capsuleRadius;
	FVector dashDirection = (targetLocation - actorLocation).GetSafeNormal() * remainingDistance / capsuleAmount;
	
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		return postTeleportLocation;
	}
	
	FVector initialPosition = postTeleportLocation;
	
	FVector differenceThresholdVector = FVector(0.f, 0.f, walkHeightDifferenceThreshold);
	FHitResult traceHitResult;
	FCollisionShape traceShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);

	for (int i = 0; i < capsuleAmount; i++)
	{
		FVector newPosition = initialPosition + dashDirection;
		world->SweepSingleByChannel(traceHitResult,
								newPosition + differenceThresholdVector,
								newPosition - differenceThresholdVector,
								FQuat::Identity, dashTraceChannel, traceShape);

		if (!traceHitResult.bBlockingHit || traceHitResult.PenetrationDepth > 0.f)
		{
			return initialPosition;
		}

		DrawDebugCapsule(world, traceHitResult.Location, capsuleHalfHeight/4.f, capsuleRadius/4.f, FQuat::Identity, FColor::Yellow, false, 5);
		initialPosition = traceHitResult.Location;
	}

	return initialPosition;
}

bool UDashAbility::DashCollidesWithObstacle(FVector dashInitialLocation, FVector dashEndLocation)
{
	UWorld* world = GetWorld();
	FHitResult traceHitResult;
	world->LineTraceSingleByChannel(traceHitResult, dashInitialLocation, dashEndLocation, dashTraceChannel);

	return traceHitResult.bBlockingHit;
}

FVector UDashAbility::GetDashEndLocation(FVector actorLocation, FVector targetLocation)
{
	if (!Init())
	{
		return FVector::ZeroVector;
	}
	
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		return FVector::ZeroVector;
	}

	FVector dashTeleportLocation = CalculateDashTeleport(actorLocation, targetLocation);
	
	FVector endOfDashLocation = CalculateDashEndPostTeleport(actorLocation, targetLocation, dashTeleportLocation);
	
	return endOfDashLocation;
}
