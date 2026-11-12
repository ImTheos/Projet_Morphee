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
	TArray<UUserWidget*> AvailableSecondaryInteractionWidgets;
	
public:
	UPROPERTY(meta = (BindWidget))
	UDialogUI* dialogUI;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(int playerHealth, bool animate = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHealthBar* healthBar;
	
	UPROPERTY(BlueprintReadWrite) 
	TSubclassOf<UUserWidget> SecondaryInteractionWidgetClass;
	UFUNCTION(BlueprintCallable)
	void InitSecondaryInteractionWidgets(const TArray<UUserWidget*>& widgets);
	UFUNCTION(BlueprintCallable)
	UUserWidget* AcquireSecondaryInteractionWidget();
	UFUNCTION(BlueprintCallable)
	void ReleaseSecondaryInteractionWidget(UUserWidget* w);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (ForceAsFunction))
	UUserWidget* ShowSecondaryIteraction(const FText& text, FVector worldLocation);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideSecondaryIteraction(const UUserWidget* w);

};
