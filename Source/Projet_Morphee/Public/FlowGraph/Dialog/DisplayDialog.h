// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION()
	void ActivateSkipButton();
	
	FScriptDelegate endDelegate;

	UPROPERTY()
	UButton* skipButton;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText dialogTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText dialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UTexture2D* leftImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UTexture2D* rightImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog", meta=(ToolTip="If activated, the text will appear progressively, each letter being delayed by the value of textDelayTime in seconds"))
	bool bDelayTextAppearance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog", meta=(EditCondition="bDelayTextAppearance", ClampMin=0))
	float textDelayTime = 0.0f;

	UFUNCTION()
	void TriggerEnd();
};
