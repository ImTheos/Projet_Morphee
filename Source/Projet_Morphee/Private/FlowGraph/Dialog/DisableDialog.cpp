// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisableDialog.h"

#include "MyCPPCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "GameLogic/UI/PlayerUI.h"

void UDisableDialog::ExecuteInput(const FName& PinName)
{
	if (!IsValid(dialogUI))
	{
		InitDialogUI();

		if (!IsValid(dialogUI))
		{
			UE_LOG(LogTemp, Error, TEXT("Dialog UI is not valid"));
			TriggerFirstOutput(true);
			return;
		}
	}
	
	dialogUI->SetVisibility(ESlateVisibility::Hidden);

	TriggerFirstOutput(true);
}
