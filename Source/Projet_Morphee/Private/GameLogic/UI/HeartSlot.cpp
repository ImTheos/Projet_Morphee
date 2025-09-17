// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/HeartSlot.h"

void UHeartSlot::SetWhiteStarVisibility(ESlateVisibility visibility) const
{
	if (IsValid(whiteStar))
	{
		whiteStar->SetVisibility(visibility);
	}
}
