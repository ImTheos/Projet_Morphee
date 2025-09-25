// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UDialogUI;
/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UDialogUI* dialogUI;

	void InitDialogUI();
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(int playerHealth, bool animate = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthBar* healthBar;
	
	void SetDialogUIVisibility(ESlateVisibility visibility);
};
