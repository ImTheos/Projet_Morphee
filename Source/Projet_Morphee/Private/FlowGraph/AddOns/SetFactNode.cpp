// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/AddOns/SetFactNode.h"



void USetFactNode::ExecuteInput(const FName& PinName)
{
	UGameManager* gameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));
	if (!gameManager)
		return;
	gameManager->SetGameFactValue(FactTag, NewValue);
	TriggerFirstOutput(true);
}
