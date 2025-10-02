// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplaySkipButton);

USTRUCT()
struct FRichTextTag
{
	GENERATED_BODY()

	FString tagName;

	int beginIndex;
	int endIndex;
};

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDialogUI : public UUserWidget
{
	GENERATED_BODY()
	
	FString parseRichText(const FText& text, TArray<FRichTextTag>& tagArray);
	FString applyTagsToString(const FString& baseString, TArray<FRichTextTag>& tagArray);

	void animateDialog(const FText& dialogText, const float delaySeconds);
	void displayPartialDialog(const FString& parsedDialogString, TArray<FRichTextTag> tagArray, const float delaySeconds, int displayedCharactersAmount);

	
public:
	void SetText(const FText& dialogText, const FText& dialogTitle, const float textDelayTime);
	void SetTextNoDelay(const FText& dialogText, const FText& dialogTitle);
	
	void setImages(UTexture2D* leftImageTexture, UTexture2D* rightImageTexture);
	
	UButton* GetSkipButton() const;

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

	FDisplaySkipButton skipButtonDelegate;
};
