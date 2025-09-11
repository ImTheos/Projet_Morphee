// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/PlayerUI.h"

#include "Animation/WidgetAnimation.h"

void UPlayerUI::UpdateHealthBar(int playerHealth)
{
	TArray<UHeartSlot*> heartSlots = healthBar->heartSlots;

	for (int i = 0; i < FMath::Min(heartSlots.Num(), playerHealth); i++)
	{
		const UHeartSlot* heartSlot = heartSlots[i];
		if (!IsValid(heartSlot))
		{
			continue;
		}
			
		UImage* WhiteStar = heartSlot->whiteStar;
		if (!IsValid(WhiteStar))
		{
			continue;
		}
		
		heartSlot->SetWhiteHeartVisibility(WhiteStar->GetVisibility());
	}

	for (int i = playerHealth; i < heartSlots.Num(); i++)
	{
		UHeartSlot* heartSlot = heartSlots[i];
		
		bool hasPlayedAnimation = false;
		if (!IsValid(heartSlot) || heartSlot->hasPlayedAnimation)
		{
			continue;
		}
		
		if (!IsValid(heartSlot) || !IsValid(heartSlot->whiteStarDissipate))
		{
			continue;
		}
		heartSlot->PlayAnimation(heartSlot->whiteStarDissipate);
		heartSlot->hasPlayedAnimation = true;

		FWidgetAnimationDynamicEvent hideWidget;
		hideWidget.BindDynamic(heartSlot, &UHeartSlot::HideWhiteHeart);

		heartSlot->whiteStarDissipate->BindToAnimationFinished(this, hideWidget);
	}
}
