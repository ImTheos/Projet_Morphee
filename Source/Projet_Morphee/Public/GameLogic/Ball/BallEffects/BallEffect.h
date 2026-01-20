// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJET_MORPHEE_API UBallEffect : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName effectName;
	
	bool operator==(const UBallEffect&) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(float deltaTime, AActor* owner);
	virtual void Tick_Implementation(float deltaTime, AActor* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Detonate(AActor* owner);
	virtual void Detonate_Implementation(AActor* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Attack(AActor* ball, AActor* attacker);
	virtual void Attack_Implementation(AActor* ball, AActor* attacker);
	
	UFUNCTION(BlueprintNativeEvent)
	void CollisionBeginOverlap(AActor* owner, UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult, bool isDamageable);
	virtual void CollisionBeginOverlap_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, 
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult, bool isDamageable);
	
	UFUNCTION(BlueprintNativeEvent)
	void CollisionBlock(AActor* owner, UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
		FVector normalImpulse, FHitResult hit, bool isDamageable);
	virtual void CollisionBlock_Implementation(AActor* owner, UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
		FVector normalImpulse, FHitResult hit, bool isDamageable);
	
	UFUNCTION(BlueprintNativeEvent)
	void EffectApplied(AActor* owner);
	virtual void EffectApplied_Implementation(AActor* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void EffectRemoved(AActor* owner);
	virtual void EffectRemoved_Implementation(AActor* owner);
	
	UFUNCTION(BlueprintCallable)
	void SpawnObject(const AActor* contextActor, UClass* spawnedClass, const FVector& location, const FRotator& rotation, ESpawnActorCollisionHandlingMethod spawnCollisionMethod,
	const ESpawnActorScaleMethod spawnScaleMethod, AActor* owner);
};

