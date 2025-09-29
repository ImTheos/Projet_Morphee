// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisplayDialog.h"

#include "GameLogic/UI/PlayerUI.h"

void UDisplayDialog::ExecuteInput(const FName& PinName)
{
	if (!IsValid(playerCharacter))
	{
		InitPlayerCharacter();

		if (!IsValid(playerCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("Player Character is not valid"));
			TriggerFirstOutput(true);
			return;
		}
	}

	// Change dialog text
	playerCharacter->playerUIWidget->SetDialogText(dialogText);

	// TODO Change first icon

	// TODO Change second icon
	
	TriggerFirstOutput(true);
}
