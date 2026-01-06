// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffect.h"

bool UBallEffect::operator==(const UBallEffect& otherEffect) const
{
	return effectName == otherEffect.effectName;
}

void UBallEffect::Tick_Implementation(float deltaTime, ABall* owner)
{
}

void UBallEffect::Detonate_Implementation(ABall* owner)
{
}

void UBallEffect::Charge_Implementation(ABall* owner)
{
}

void UBallEffect::Collide_Implementation(AActor* collidedActor, UActorComponent* collidedComponent, ABall* owner)
{
}
