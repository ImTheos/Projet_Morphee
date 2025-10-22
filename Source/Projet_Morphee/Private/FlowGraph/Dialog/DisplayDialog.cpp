// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisplayDialog.h"

void UDisplayDialog::ExecuteInput(const FName& PinName)
{
	TryInitialize();

	if (!IsValid(dialogUI))
	{
		UE_LOG(LogTemp, Error, TEXT("dialogUI is not valid"));
		TriggerFirstOutput(true);
		return;
	}

	skipButton = dialogUI->skipButton;
	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		TriggerFirstOutput(true);
		return;
	}
	
	// Hides button
	skipButton->SetVisibility(ESlateVisibility::Hidden);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Hidden);
	
	// binds skip button activation, used to display the skip button when the whole text will be displayed
	dialogUI->displaySkipButtonDelegate.AddDynamic(this, &UDisplayDialog::ActivateSkipButton);

	// Change dialog text and character name, animates the text apperance if this feature is activated
	if (bDelayTextAppearance)
	{
		dialogUI->SetText(dialogText, characterName, textDelayTime);
	}
	else
	{
		dialogUI->SetTextNoDelay(dialogText, characterName);
	}
	
	// change characters icons
	dialogUI->setImages(leftImage, rightImage);
}

void UDisplayDialog::ActivateSkipButton()
{
	dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDisplayDialog::ActivateSkipButton);
	
	skipButton = dialogUI->GetSkipButton();

	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		return;
	}
	
	skipButton->OnClicked.AddDynamic(this, &UDisplayDialog::TriggerEnd);

	// Shows button
	skipButton->SetVisibility(ESlateVisibility::Visible);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Visible);
}

void UDisplayDialog::TriggerEnd()
{
	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		TriggerFirstOutput(true);
		return;
	}
	
	skipButton->OnClicked.RemoveDynamic(this, &UDisplayDialog::TriggerEnd);
	
	UE_LOG(LogTemp, Display, TEXT("Trigger End reached"));
	TriggerFirstOutput(true);
}
