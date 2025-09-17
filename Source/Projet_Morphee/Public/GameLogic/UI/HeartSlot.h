// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HeartSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UHeartSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWhiteStarVisibility(ESlateVisibility visibility) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UImage* whiteStar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* dissipateAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* resplenishAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool visible = true;
};
