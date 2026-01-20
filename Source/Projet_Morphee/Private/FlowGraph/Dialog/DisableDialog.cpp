// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DisableDialog.h"

#include "FlowSubsystem.h"
#include "MyCPPCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "GameLogic/UI/PlayerUI.h"
#include "Kismet/GameplayStatics.h"

bool UDisableDialog::TrySetGameAndUIInputMode()
{
	const TSet<TWeakObjectPtr<AMyCPPCharacter>> playerCharacterSet = GetFlowSubsystem()->GetActors<AMyCPPCharacter>(playerTag);

	if (playerCharacterSet.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("DisableDialog FlowNode : no PlayerCharacter has required tag : %s"), *playerTag.ToString());
		return false;
	}

	const AMyCPPCharacter* playerCharacter = playerCharacterSet.Array()[0].Get();

	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("DisableDialog FlowNode : invalid PlayerCharacter"));
		return false;
	}

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Error, TEXT("DisableDialog FlowNode : invalid PlayerController"));
		return false;
	}

	playerController->SetInputMode(FInputModeGameAndUI());
	playerController->FlushPressedKeys();
	return true;
}

void UDisableDialog::ExecuteInput(const FName& PinName)
{
	if (!TryInitialize() || !TrySetGameAndUIInputMode())
	{
		TriggerFirstOutput(true);
		return;
	}
	
	dialogUI->SetVisibility(ESlateVisibility::Hidden);

	TriggerFirstOutput(true);
}
