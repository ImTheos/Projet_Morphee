// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/DialogUI.h"

FString UDialogUI::parseRichText(const FText& text, TArray<FRichTextTag>& tagArray)
{
	FString processedText;

	FRegexPattern regexPattern(TEXT(R"(([^<]+)|<([^>]+)>(.*?)</>)"));

	FRegexMatcher regexMatcher(regexPattern, text.ToString());

	int processedTextLength = 0;

	while (regexMatcher.FindNext())
	{
		FString regularText = regexMatcher.GetCaptureGroup(1); // regular text
		FString tagName = regexMatcher.GetCaptureGroup(2); // tag name
		FString taggedText = regexMatcher.GetCaptureGroup(3); // text inside tag
		
		if (!tagName.IsEmpty() && !taggedText.IsEmpty())
		{
			FRichTextTag newTag = FRichTextTag(	tagName,
												processedText.Len() + regularText.Len(),
												processedText.Len() + regularText.Len() + taggedText.Len());

			tagArray.Add(newTag);
		}
		
		processedText += regularText + taggedText;

		processedTextLength += regularText.Len() + taggedText.Len();
	}
	
	return processedText;
}

FString UDialogUI::applyTagsToString(const FString& baseString, TArray<FRichTextTag>& tagArray)
{
	FString resultString;
	int tagIndex = 0;

	bool openTag = false;

	for (int i = 0; i < baseString.Len(); i++)
	{
		if (tagIndex < tagArray.Num() && i == tagArray[tagIndex].beginIndex)
		{
			resultString += "<" + tagArray[tagIndex].tagName + ">";
			openTag = true;
		}
		if (tagIndex < tagArray.Num() && i == tagArray[tagIndex].endIndex)
		{
			resultString += "</>";
			tagIndex++;
			openTag = false;
		}

		resultString += baseString[i];
	}

	if (openTag)
	{
		resultString += "</>";
	}

	return resultString;
}

void UDialogUI::animateDialog(const FText& dialogText, const float delaySeconds)
{
	auto tagArray = TArray<FRichTextTag>();

	FString parsedDialogString = parseRichText(dialogText, tagArray);

	displayPartialDialog(parsedDialogString, tagArray, delaySeconds, 0);
}

void UDialogUI::displayPartialDialog(const FString& parsedDialogString, TArray<FRichTextTag> tagArray, const float delaySeconds, int displayedCharactersAmount)
{
	if (displayedCharactersAmount > parsedDialogString.Len())
	{
		displaySkipButtonDelegate.Broadcast();
		return;
	}

	FString choppedRichString = applyTagsToString(parsedDialogString.LeftChop(parsedDialogString.Len() - displayedCharactersAmount), tagArray);

	dialogTextBlock->SetText(FText::FromString(choppedRichString));

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this, parsedDialogString, tagArray, delaySeconds, displayedCharactersAmount]()
	{
		displayPartialDialog(parsedDialogString, tagArray, delaySeconds, displayedCharactersAmount + 1);
	}, delaySeconds, false);
}

void UDialogUI::SetText(const FText& dialogText, const FText& dialogTitle, const float textDelayTime)
{
	if (!dialogTextBlock || !dialogCharacterName)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog textblock"));
		return;
	}

	dialogCharacterName->SetText(dialogTitle);
	animateDialog(dialogText, textDelayTime);
}

void UDialogUI::SetTextNoDelay(const FText& dialogText, const FText& dialogTitle)
{
	if (!dialogTextBlock || !dialogCharacterName)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog textblock"));
		return;
	}

	dialogCharacterName->SetText(dialogTitle);
	dialogTextBlock->SetText(dialogText);
	displaySkipButtonDelegate.Broadcast();
}

void UDialogUI::setImages(UTexture2D* leftImageTexture, UTexture2D* rightImageTexture)
{
	if (!leftImage || !rightImage)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog image"));
		return;
	}

	if (IsValid(leftImageTexture))
	{
		leftImage->SetVisibility(ESlateVisibility::Visible);
		leftImage->SetBrushFromTexture(leftImageTexture);
	}
	else
	{
		leftImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(rightImageTexture))
	{
		rightImage->SetVisibility(ESlateVisibility::Visible);
		rightImage->SetBrushFromTexture(rightImageTexture);
	}
	else
	{
		rightImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

UButton* UDialogUI::GetSkipButton() const
{
	return skipButton;
}
