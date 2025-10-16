// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDashAbility : public UGameplayAbility
{
	GENERATED_BODY()

	// Initialize all variables based on the player avatar
	bool Init();

	// Calculates the position of the player after the first part of the dash where he teleports
	FVector CalculateDashTeleport(FVector actorLocation, FVector targetLocation);

	// Calculates the position of the player after the second part of the dash where he sprint forward (i.e. the player can't jump across holes)
	FVector CalculateDashEndPostTeleport(FVector actorLocation, FVector targetLocation, FVector postTeleportLocation);

	// Tests if there is an obstacle for dashTraceChannel between dashInitialLocation and dashEndLocation
	bool DashCollidesWithObstacle(FVector dashInitialLocation, FVector dashEndLocation);

	float capsuleRadius;
	float capsuleHalfHeight;

public:
	// Returns the position where the player should dash if he tries to go from actorLocation to targetLocation
	UFUNCTION(BlueprintCallable)
	FVector GetDashEndLocation(FVector actorLocation, FVector targetLocation);

	// Step in height allowed between the player initial position and the position after the dash teleportation
	UPROPERTY(EditAnywhere)
	float teleportHeightDifferenceThreshold = 20.f;

	// Step in height allowed in slopes between two player capsules
	UPROPERTY(EditAnywhere)
	float walkHeightDifferenceThreshold = 20.f;

	// Maximum distance for the player dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashMaxDistance = 400.f;

	// Collision channel used for wall and obstacle detection during dash
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> dashTraceChannel = ECC_Camera;
	// TODO : change collision channel to match gameplay intentions
};
