// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisplayDialog.h"

void UDisplayDialog::ExecuteInput(const FName& PinName)
{
	if (!IsValid(dialogUI))
	{
		InitDialogUI();

		if (!IsValid(dialogUI))
		{
			UE_LOG(LogTemp, Error, TEXT("dialogUI is not valid"));
			TriggerFirstOutput(true);
			return;
		}
	}

	// Change dialog text and character name
	if (bDelayTextAppearance)
	{
		dialogUI->SetText(dialogText, dialogTitle, textDelayTime);
	}
	else
	{
		dialogUI->SetTextNoDelay(dialogText, dialogTitle);
	}
	
	// change characters icons
	dialogUI->setImages(leftImage, rightImage);

	// bind skip button activation
	dialogUI->skipButtonDelegate.AddDynamic(this, &UDisplayDialog::ActivateSkipButton);
}

void UDisplayDialog::ActivateSkipButton()
{
	// TODO Visually indicate that the button is activated
	
	skipButton = dialogUI->GetSkipButton();
	endDelegate.BindUFunction(this, "TriggerEnd");
	skipButton->OnClicked.Add(endDelegate);
}

void UDisplayDialog::TriggerEnd()
{
	skipButton->OnClicked.RemoveDynamic(this, &UDisplayDialog::TriggerEnd);
	
	UE_LOG(LogTemp, Display, TEXT("Trigger End reached"));
	TriggerFirstOutput(true);
}
