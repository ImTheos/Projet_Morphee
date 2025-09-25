// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/ActivateDialog.h"

#include "FlowSubsystem.h"
#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"

void UActivateDialog::ExecuteInput(const FName& PinName)
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
	
	playerCharacter->playerUIWidget->SetDialogUIVisibility(ESlateVisibility::Visible);
	
	// TODO : ajouter la mise en pause des contrôles du joueur pendant le dialogue (?)

	TriggerFirstOutput(true);
}
