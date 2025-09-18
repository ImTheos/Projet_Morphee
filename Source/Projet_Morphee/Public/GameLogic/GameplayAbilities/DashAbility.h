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

	bool Init();
	FVector CalculateDashTeleport(FVector actorLocation, FVector targetLocation);

	float capsuleRadius;
	float capsuleHalfHeight;

public:
	FVector CalculateDashEndPostTeleport(FVector actorLocation, FVector targetLocation, FVector postTeleportLocation);
	UFUNCTION(BlueprintCallable)
	FVector GetDashEndLocation(FVector actorLocation, FVector targetLocation);

	UPROPERTY(EditAnywhere)
	float heightDifferenceThreshold = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashMaxDistance = 400.f;
};
