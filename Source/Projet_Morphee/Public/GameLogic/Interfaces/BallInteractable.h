// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BallInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBallInteractable : public UInterface
{
	GENERATED_BODY()
};


class PROJET_MORPHEE_API IBallInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCollideWithBall(FVector collisionLocation);
	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDetonatedByBall(FVector detonationLocation);
};
