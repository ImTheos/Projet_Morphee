// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowGraph/Dialog/DialogBase.h"
#include "EnableDialog.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UEnableDialog : public UDialogBase
{
	GENERATED_BODY()

	bool TrySetUIOnlyInputMode();

	virtual void ExecuteInput(const FName& PinName) override;
};
