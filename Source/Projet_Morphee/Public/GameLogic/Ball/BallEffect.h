// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "UObject/NoExportTypes.h"
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
	void Tick(float deltaTime, ABall* owner);
	virtual void Tick_Implementation(float deltaTime, ABall* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Detonate(ABall* owner);
	virtual void Detonate_Implementation(ABall* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Charge(ABall* owner);
	virtual void Charge_Implementation(ABall* owner);
	
	UFUNCTION(BlueprintNativeEvent)
	void Collide(AActor* collidedActor, UActorComponent* collidedComponent, ABall* owner);
	virtual void Collide_Implementation(AActor* collidedActor, UActorComponent* collidedComponent, ABall* owner);
};
