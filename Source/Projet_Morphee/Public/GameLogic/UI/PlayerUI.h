// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
	void UpdateHealthBar(int playerHealth);

public:
	UPROPERTY(BlueprintReadWrite)
	UHealthBar* healthBar;
};
