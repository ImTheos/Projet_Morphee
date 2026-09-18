// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowGraph/Nodes/SetFact_FlowNode.h"



void USetFact_FlowNode::ExecuteInput(const FName& PinName)
{
	UGameManager* gameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));
	if (!gameManager)
		return;
	gameManager->SetGameFactValue(FactTag, NewValue);
	TriggerFirstOutput(true);
}
