// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UDialogUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndPlayFreeDialog);


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
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="FreeDialog")
	void PlayFreeDialog(const FText& text, const float& duration);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="FreeDialog")
	FEndPlayFreeDialog OnEndPlayFreeDialogDelegate;
	
	UPROPERTY(BlueprintReadWrite, Category="SecondaryInteraction") 
	TSubclassOf<UUserWidget> SecondaryInteractionWidgetClass;
	UFUNCTION(BlueprintCallable, Category="SecondaryInteraction")
	void InitSecondaryInteractionWidgets(const TArray<UUserWidget*>& widgets);
	UFUNCTION(BlueprintCallable, Category="SecondaryInteraction")
	UUserWidget* AcquireSecondaryInteractionWidget();
	UFUNCTION(BlueprintCallable, Category="SecondaryInteraction")
	void ReleaseSecondaryInteractionWidget(UUserWidget* w);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (ForceAsFunction), Category="SecondaryInteraction")
	UUserWidget* ShowSecondaryIteraction(const FText& text, FVector worldLocation);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="SecondaryInteraction")
	void HideSecondaryIteraction(const UUserWidget* w);

};
