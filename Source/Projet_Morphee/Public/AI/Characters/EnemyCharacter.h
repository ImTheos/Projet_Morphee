// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// Base class for enemies, that gives root logic for AI behaviours
UCLASS()
class PROJET_MORPHEE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	
	UFUNCTION(BlueprintCallable)
	virtual void InitCharacter();
};
