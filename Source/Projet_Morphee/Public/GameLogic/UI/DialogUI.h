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

// Struct used to store Rich Text tags, their name and the characters they apply to
USTRUCT()
struct FRichTextTag
{
	GENERATED_BODY()

	FString tagName;
	
	// Tag will apply to characters inside [beginIndex;endIndex[ 
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

	// returns the String without its tags and fills tagArray with all the removed tags information
	FString parseRichText(const FText& text, TArray<FRichTextTag>& tagArray);

	// apply tags on eligible characters of baseString
	// tags will apply to characters inside their [beginIndex;endIndex[ 
	FString applyTagsToString(const FString& baseString, TArray<FRichTextTag>& tagArray);

	// displays dialogText letter after letter, with a cooldown of delaySeconds before each new letter appears
	void animateDialog(const FText& dialogText, const float delaySeconds);
	
	// displays the displayedCharactersAmount first characters of parsedDialogString
	// and sets a timer to display the next character of the string in delaySeconds seconds
	void displayPartialDialog(const FString& parsedDialogString, TArray<FRichTextTag> tagArray, const float delaySeconds, int displayedCharactersAmount);

public:

	// Displays dialogTitle in the dialog title field and
	// displays dialogText letter after letter inside the dialog text field,
	// with a cooldown of textDelaytime before each new letter appears
	void SetText(const FText& dialogText, const FText& dialogTitle, const float textDelayTime);

	// Displays dialogTitle in the dialog title field and
	// displays dialogText letter after letter inside the dialog text field
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

	UPROPERTY(meta = (BindWidget))
	UImage* skipLogo;

	// delegate called when the skip button should be displayed on screen
	FDisplaySkipButton displaySkipButtonDelegate;
};
