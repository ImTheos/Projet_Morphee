// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffect.h"

bool UBallEffect::operator==(const UBallEffect& otherEffect) const
{
	return effectName == otherEffect.effectName;
}

void UBallEffect::Tick_Implementation(float deltaTime, AActor* owner)
{
}

void UBallEffect::Detonate_Implementation(AActor* owner)
{
}

void UBallEffect::Charge_Implementation(AActor* owner)
{
}

void UBallEffect::Collide_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
}
