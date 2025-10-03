// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Dialog/DialogBase.h"

#include "FlowSubsystem.h"
#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"

// Hard coding of default player tag
UE_DEFINE_GAMEPLAY_TAG(DefaultPlayerTag, "Flow.Identify.MyPlayer");

bool UDialogBase::TryInitialize()
{
	const TSet<TWeakObjectPtr<AMyCPPCharacter>> playerCharacterSet = GetFlowSubsystem()->GetActors<AMyCPPCharacter>(playerTag);

	if (playerCharacterSet.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("DialogBase FlowNode : no PlayerCharacter has required tag : %s"), *playerTag.ToString());
		return false;
	}

	const AMyCPPCharacter* playerCharacter = playerCharacterSet.Array()[0].Get();

	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("DialogBase FlowNode : invalid PlayerCharacter"));
		return false;
	}

	const UPlayerUI* playerUI = playerCharacter->playerUIWidget;
	if (!IsValid(playerUI))
	{
		UE_LOG(LogTemp, Error, TEXT("DialogBase FlowNode : invalid PlayerUI"));
		return false;
	}

	dialogUI = playerUI->dialogUI;

	if (!IsValid(dialogUI))
	{
		UE_LOG(LogTemp, Error, TEXT("DialogBase FlowNode : invalid DialogUI"));
		return false;
	}

	return true;
}
