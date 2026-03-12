// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffect.h"
#include "NiagaraSystem.h"
#include "Explode.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API AExplode : public ABallEffect
{
	GENERATED_BODY()
	
	virtual void Detonate_Implementation() override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* explosionParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float explosionRange = 300.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float explosionDamage = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> explosionCollisionChannel;
	
};
