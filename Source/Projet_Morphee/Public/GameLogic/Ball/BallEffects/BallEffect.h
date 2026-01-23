// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffect.generated.h"

class ABall;

UENUM(BlueprintType)
enum class ESpawnResult : uint8
{
	SpawnResult_Success UMETA(DisplayName="Success"),
	SpawnResult_Failed UMETA(DisplayName="Spawn Failed"),
};

/**
 * 
 */
UCLASS(Blueprintable)
class PROJET_MORPHEE_API ABallEffect : public AActor
{
	GENERATED_BODY()
	
public:
	ABallEffect();
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn = true))
	ABall* effectOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName effectName;
	
	bool operator==(const ABallEffect&) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void EffectTick(float deltaTime);
	virtual void EffectTick_Implementation(float deltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void Detonate();
	virtual void Detonate_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void Attack(AActor* attacker);
	virtual void Attack_Implementation(AActor* attacker);
	
	UFUNCTION(BlueprintNativeEvent)
	void CollisionBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	                           int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult, bool isDamageable);
	virtual void CollisionBeginOverlap_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, 
	                                                  int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult, bool isDamageable);
	
	UFUNCTION(BlueprintNativeEvent)
	void CollisionBlock(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
	                    FVector normalImpulse, FHitResult hit, bool isDamageable);
	virtual void CollisionBlock_Implementation(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
	                                           FVector normalImpulse, FHitResult hit, bool isDamageable);
	
	UFUNCTION(BlueprintNativeEvent)
	void EffectApplied();
	virtual void EffectApplied_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void EffectRemoved();
	virtual void EffectRemoved_Implementation();
	
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "OutputPins", ReturnDisplayName="Spawned Object"))
	AActor* SpawnObject(const AActor* contextActor, UClass* spawnedClass, ESpawnResult& OutputPins, const FVector location = FVector::ZeroVector, const FRotator rotation = FRotator::ZeroRotator, ESpawnActorCollisionHandlingMethod spawnCollisionMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
	const ESpawnActorScaleMethod spawnScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot, AActor* objectOwner = nullptr);
};

