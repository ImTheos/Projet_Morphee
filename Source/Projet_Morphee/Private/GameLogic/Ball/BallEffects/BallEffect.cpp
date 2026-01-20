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

void UBallEffect::CollisionBeginOverlap_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent,
	AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep,
	const FHitResult& sweepResult, bool isDamageable)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::CollisionBeginOverlap was triggered"))
}

void UBallEffect::CollisionBlock_Implementation(AActor* owner, UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
		FVector normalImpulse, FHitResult hit, bool isDamageable)
{
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::CollisionBlock was triggered"))
}

void UBallEffect::EffectApplied_Implementation(AActor* owner)
{
	FString displayName = effectName.ToString().IsEmpty() ? GetClass()->GetName() : effectName.ToString();
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::EffectApplied was triggered for effect %s"), *displayName)
}

void UBallEffect::EffectRemoved_Implementation(AActor* owner)
{
	FString displayName = effectName.ToString().IsEmpty() ? GetClass()->GetName() : effectName.ToString();
	UE_LOG(LogTemp, Display, TEXT("UBallEffect::EffectRemoved was triggered for effect %s"), *displayName)
}

void UBallEffect::SpawnObject(const AActor* contextActor, UClass* spawnedClass, const FVector& location, const FRotator& rotation, ESpawnActorCollisionHandlingMethod spawnCollisionMethod,
	const ESpawnActorScaleMethod spawnScaleMethod, AActor* owner)
{
	UWorld* world = contextActor->GetWorld();
	
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = spawnCollisionMethod;
	spawnParams.Owner = owner;
	spawnParams.TransformScaleMethod = spawnScaleMethod;
	
	world->SpawnActor(spawnedClass, &location, &rotation, spawnParams);
}
