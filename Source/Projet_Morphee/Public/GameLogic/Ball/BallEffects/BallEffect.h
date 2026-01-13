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
	void Charge(AActor* owner);
	virtual void Charge_Implementation(AActor* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Collide(AActor* owner, UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	virtual void Collide_Implementation(AActor* owner, UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
};
