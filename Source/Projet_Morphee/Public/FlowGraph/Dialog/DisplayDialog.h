// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "FlowGraph/Dialog/DialogBase.h"
#include "DisplayDialog.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDisplayDialog : public UDialogBase
{
	GENERATED_BODY()

	virtual void ExecuteInput(const FName& PinName) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText dialogTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText dialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UTexture2D* leftImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UTexture2D* rightImage;

	UFUNCTION()
	void TriggerEnd();
};
