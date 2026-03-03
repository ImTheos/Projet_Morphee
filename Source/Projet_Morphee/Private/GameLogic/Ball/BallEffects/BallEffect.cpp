// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffects/BallEffect.h"

ABallEffect::ABallEffect()
{
	effectName = FName(GetClass()->GetName());
}

bool ABallEffect::operator==(const ABallEffect& otherEffect) const
{
	return effectName == otherEffect.effectName;
}

void ABallEffect::EffectTick_Implementation(float deltaTime)
{
	
}

void ABallEffect::Detonate_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("ABallEffect::Detonate was triggered"))
}

void ABallEffect::Attack_Implementation(AActor* attacker)
{
	UE_LOG(LogTemp, Display, TEXT("UBalEffect::Attack was triggered"))
}

void ABallEffect::CollisionBeginOverlap_Implementation(UPrimitiveComponent* overlappedComponent,
                                                       AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep,
                                                       const FHitResult& sweepResult, bool isDamageable)
{
	UE_LOG(LogTemp, Display, TEXT("ABallEffect::CollisionBeginOverlap was triggered"))
}

void ABallEffect::CollisionBlock_Implementation(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
                                                FVector normalImpulse, FHitResult hit, bool isDamageable)
{
	UE_LOG(LogTemp, Display, TEXT("ABallEffect::CollisionBlock was triggered"))
}

void ABallEffect::EffectApplied_Implementation()
{
	FString displayName = effectName.ToString().IsEmpty() ? GetClass()->GetName() : effectName.ToString();
	UE_LOG(LogTemp, Display, TEXT("ABallEffect::EffectApplied was triggered for effect %s"), *displayName)
}

void ABallEffect::EffectRemoved_Implementation()
{
	FString displayName = effectName.ToString().IsEmpty() ? GetClass()->GetName() : effectName.ToString();
	UE_LOG(LogTemp, Display, TEXT("ABallEffect::EffectRemoved was triggered for effect %s"), *displayName)
}

AActor* ABallEffect::SpawnObject(const AActor* contextActor, UClass* spawnedClass, ESpawnResult& OutputPins, const FVector location, const FRotator rotation, ESpawnActorCollisionHandlingMethod spawnCollisionMethod,
	const ESpawnActorScaleMethod spawnScaleMethod, AActor* objectOwner)
{
	if (!IsValid(contextActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ABallEffect::SpawnObject : invalid contextActor"))
		OutputPins = ESpawnResult::SpawnResult_Failed;
		return nullptr;
	}
	
	UWorld* world = contextActor->GetWorld();
	
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = spawnCollisionMethod;
	spawnParams.Owner = objectOwner;
	spawnParams.TransformScaleMethod = spawnScaleMethod;
	
	AActor* spawnedActor = world->SpawnActor(spawnedClass, &location, &rotation, spawnParams);
	
	if (!IsValid(spawnedActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ABallEffect::SpawnObject : spawned actor was invalid"))
		OutputPins = ESpawnResult::SpawnResult_Failed;
		return nullptr;
	}
	
	OutputPins = ESpawnResult::SpawnResult_Success;
	return spawnedActor;
}
