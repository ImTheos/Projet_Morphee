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

	// TODO : remove following line and set a real value for the threshold
	heightDifferenceThreshold = capsuleHalfHeight;

	return true;
}

FVector UDashAbility::CalculateDashTeleport(FVector actorLocation, FVector targetLocation)
{
	UWorld* world = GetWorld();

	FVector differenceThresholdVector = FVector(0.f, 0.f, heightDifferenceThreshold);
	ECollisionChannel traceChannel = ECC_Camera;
	// TODO : change collision channel to match gameplay intentions
	
	FHitResult traceHitResult;
	FCollisionShape traceShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);
	
	world->SweepSingleByChannel(traceHitResult,
								targetLocation + differenceThresholdVector,
								targetLocation - differenceThresholdVector,
								FQuat::Identity, traceChannel, traceShape);

	if (traceHitResult.bBlockingHit)
	{
		DrawDebugCapsule(world, traceHitResult.Location, capsuleHalfHeight, capsuleRadius, FQuat::Identity, FColor::Purple, false, 10);
		return traceHitResult.Location;
	}
	
	return actorLocation;
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

	
	
	//FVector endOfDashLocation = CalculateDashEndPostTeleport(dashTeleportLocation);
	
	return FVector::Zero();
}
