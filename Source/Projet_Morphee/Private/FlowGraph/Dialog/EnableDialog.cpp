// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/EnableDialog.h"

#include "FlowSubsystem.h"
#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"
#include "Kismet/GameplayStatics.h"

bool UEnableDialog::TrySetUIOnlyInputMode()
{
	const TSet<TWeakObjectPtr<AMyCPPCharacter>> playerCharacterSet = GetFlowSubsystem()->GetActors<AMyCPPCharacter>(playerTag);

	if (playerCharacterSet.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : no PlayerCharacter has required tag : %s"), *playerTag.ToString());
		return false;
	}

	const AMyCPPCharacter* playerCharacter = playerCharacterSet.Array()[0].Get();

	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : invalid PlayerCharacter"));
		return false;
	}

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : invalid PlayerController"));
		return false;
	}

	playerController->SetInputMode(FInputModeUIOnly());
	playerController->FlushPressedKeys();
	return true;
}

void UEnableDialog::ExecuteInput(const FName& PinName)
{
	if (!TryInitialize() || !TrySetUIOnlyInputMode())
	{
		TriggerFirstOutput(true);
		return;
	}
	
	dialogUI->SetVisibility(ESlateVisibility::Visible);

	TriggerFirstOutput(true);
}
