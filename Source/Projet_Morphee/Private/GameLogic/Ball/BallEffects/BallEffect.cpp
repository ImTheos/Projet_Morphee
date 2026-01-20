// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffects/BallEffect.h"

bool UBallEffect::operator==(const UBallEffect& otherEffect) const
{
	return effectName == otherEffect.effectName;
}

void UBallEffect::Tick_Implementation(float deltaTime, AActor* owner)
{
	
}

void UBallEffect::Detonate_Implementation(AActor* owner)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::Detonate was triggered"))
}

void UBallEffect::Attack_Implementation(AActor* ball, AActor* attacker)
{
	UE_LOG(LogTemp, Display, TEXT("UBalEffect::Attack was triggered"))
}

void UBallEffect::CollideDamageable_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent,
	AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep,
	const FHitResult& sweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::CollideDamageable was triggered"))
}

void UBallEffect::CollideNotDamageable_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent,
	AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep,
	const FHitResult& sweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::CollideNotDamageable was triggered"))
}

void UBallEffect::EffectApplied_Implementation(AActor* owner)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::EffectApplied was triggered"))
}
