// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/DialogUI.h"

void UDialogUI::SetText(const FText& dialogText, const FText& dialogTitle)
{
	if (!dialogTextBlock || !dialogCharacterName)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogUI : invalid dialog textblock"));
		return;
	}

	dialogTextBlock->SetText(dialogText);
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
