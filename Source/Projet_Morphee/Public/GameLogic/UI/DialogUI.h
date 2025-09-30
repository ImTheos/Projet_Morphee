// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "FlowGraph/Dialog/DisplayDialog.h"
#include "DialogUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDialogUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText(const FText& dialogText, const FText& dialogTitle);
	void setImages(UTexture2D* leftImageTexture, UTexture2D* rightImageTexture);
	void BindButtonToEnd(UDisplayDialog* dialogToEnd);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* dialogCharacterName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* dialogTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* leftImage;

	UPROPERTY(meta = (BindWidget))
	UImage* rightImage;

	UPROPERTY(meta = (BindWidget))
	UButton* skipButton;
};
