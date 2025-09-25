// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisableDialog.h"

#include "MyCPPCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "GameLogic/UI/PlayerUI.h"

void UDisableDialog::ExecuteInput(const FName& PinName)
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
	
	playerCharacter->playerUIWidget->SetDialogUIVisibility(ESlateVisibility::Hidden);

	TriggerFirstOutput(true);
}
