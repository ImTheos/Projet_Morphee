// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/DialogUI.h"

FString UDialogUI::parseRichText(const FText& text, TArray<FRichTextTag>& tagArray)
{
	FString resultString;

	FRegexPattern regexPattern(TEXT(R"(([^<]+)|<([^>]+)>(.*?)</>)"));

	FRegexMatcher regexMatcher(regexPattern, text.ToString());

	int charCounter = 0;

	while (regexMatcher.FindNext())
	{
		FString captureGroup1 = regexMatcher.GetCaptureGroup(1); // regular text
		FString captureGroup2 = regexMatcher.GetCaptureGroup(2); // tag name
		FString captureGroup3 = regexMatcher.GetCaptureGroup(3); // text inside tag

		resultString += captureGroup1 + captureGroup3;
		
		if (!captureGroup2.IsEmpty() && !captureGroup3.IsEmpty())
		{
			FRichTextTag newTag = FRichTextTag(	captureGroup2,
												charCounter + captureGroup1.Len(),
												charCounter + captureGroup1.Len() + captureGroup3.Len());

			tagArray.Add(newTag);
		}

		charCounter += captureGroup1.Len() + captureGroup3.Len();
	}
	
	return resultString;
}

FString UDialogUI::applyTagsToString(const FString& baseString, TArray<FRichTextTag>& tagArray)
{
	FString resultString;
	int tagIndex = 0;

	for (int i = 0; i < baseString.Len(); i++)
	{
		if (tagIndex < tagArray.Num() && i == tagArray[tagIndex].beginIndex)
		{
			resultString += "<" + tagArray[tagIndex].tagName + ">";
		}
		if (tagIndex < tagArray.Num() && i == tagArray[tagIndex].endIndex)
		{
			resultString += "</>";
			tagIndex++;
		}

		resultString += baseString[i];
	}

	return resultString;
}

void UDialogUI::animateDialog(const FText& dialogText, const float delaySeconds)
{
	auto tagArray = TArray<FRichTextTag>();

	FString parsedString = parseRichText(dialogText, tagArray);
	
	for (int i = parsedString.Len() - 1; i >= 0; i++)
	{
		FString choppedRichString = applyTagsToString(parsedString.RightChop(i), tagArray);

		FTimerHandle timerHandle;
		// GetWorld()->GetTimerManager().SetTimer(timerHandle, [parsedString, tagArray]()
		// {
		// 	// faire un appel récursif
		// });
		dialogTextBlock->SetText(FText::FromString(choppedRichString));
	}
}

void UDialogUI::SetText(const FText& dialogText, const FText& dialogTitle)
{
	if (!dialogTextBlock || !dialogCharacterName)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog textblock"));
		return;
	}

	dialogCharacterName->SetText(dialogTitle);
}

void UDialogUI::setImages(UTexture2D* leftImageTexture, UTexture2D* rightImageTexture)
{
	if (!leftImage || !rightImage)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog image"));
		return;
	}

	leftImage->SetBrushFromTexture(leftImageTexture);
	rightImage->SetBrushFromTexture(rightImageTexture);
}

void UDialogUI::BindButtonToEnd(UDisplayDialog* dialogToEnd)
{
	FScriptDelegate endDelegate;
	endDelegate.BindUFunction(dialogToEnd, "TriggerEnd");
	skipButton->OnClicked.Add(endDelegate);
}
