// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowGraph/Dialog/DialogBase.h"
#include "DisableDialog.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDisableDialog : public UDialogBase
{
	GENERATED_BODY()

	virtual void ExecuteInput(const FName& PinName) override;
};
