// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/PlayerUI.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GameLogic/UI/DialogUI.h"

void UPlayerUI::UpdateHealthBar(int playerHealth, bool animate)
{
	TArray<UHeartSlot*> heartSlots = healthBar->heartSlots;

	for (int i = 0; i < FMath::Min(heartSlots.Num(), playerHealth); i++)
	{
		UHeartSlot* heartSlot = heartSlots[i];
		if (!IsValid(heartSlot))
		{
			continue;
		}
		
		if (!IsValid(heartSlot->resplenishAnim) || !IsValid(heartSlot->dissipateAnim))
		{
			continue;
		}

		if (heartSlot->IsAnimationPlaying(heartSlot->dissipateAnim))
		{
			heartSlot->StopAllAnimations();
		}

		if (heartSlot->visible)
		{
			continue;
		}
		
		float animTimeStart = animate ? 0 : heartSlot->resplenishAnim->GetEndTime();
		heartSlot->PlayAnimation(heartSlot->resplenishAnim, animTimeStart);
		heartSlot->visible = true;
	}

	for (int i = playerHealth; i < heartSlots.Num(); i++)
	{
		UHeartSlot* heartSlot = heartSlots[i];
		if (!IsValid(heartSlot))
		{
			continue;
		}
		
		if (!IsValid(heartSlot->dissipateAnim) || !IsValid(heartSlot->dissipateAnim))
		{
			continue;
		}

		if (heartSlot->IsAnimationPlaying(heartSlot->resplenishAnim))
		{
			heartSlot->StopAllAnimations();
		}
		
		if (!heartSlot->visible)
		{
			continue;
		}
		
		float animTimeStart = animate ? 0 : heartSlot->dissipateAnim->GetEndTime();
		heartSlot->PlayAnimation(heartSlot->dissipateAnim, animTimeStart);
		heartSlot->visible = false;
	}
}