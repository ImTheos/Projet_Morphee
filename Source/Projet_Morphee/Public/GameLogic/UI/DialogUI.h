// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "DialogUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDialogUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	URichTextBlock* dialogTextBlock;

	void InitTextBlock();
	
public:
	void SetText(const FText& dialogText);
};
