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

	// Triggers the node main logic
	virtual void ExecuteInput(const FName& PinName) override;
	
	UPROPERTY()
	UButton* skipButton;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText characterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText dialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Images")
	UTexture2D* leftImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Images")
	UTexture2D* rightImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta=(ToolTip="If activated, the text will appear progressively, each letter being delayed by the value of textDelayTime in seconds"))
	bool bDelayTextAppearance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta=(EditCondition="bDelayTextAppearance", ClampMin=0))
	float textDelayTime = 0.05f;

private:
	UFUNCTION()
	void ActivateSkipButton();
	
public:
	UFUNCTION()
	void TriggerEnd();
};
