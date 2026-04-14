// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowGraph/Dialog/DialogBase.h"
#include "DisplayDialogv2.generated.h"

USTRUCT(BlueprintType)
struct FDialogData
{
	GENERATED_BODY()
	
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
};

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UDisplayDialogv2 : public UDialogBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	UButton* skipButton;
	
	bool TrySetInputMode(const FInputModeDataBase& inputMode);

	virtual void ExecuteInput(const FName& PinName) override;

	UFUNCTION()
	void ActivateSkipButton();
	
	UFUNCTION()
	void TriggerSkip();
	
	UFUNCTION()
	void EndDialog();
	
	UFUNCTION()
	void DisplayNextDialogLine();
	
	int currentDialogIndex = 0;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TArray<FDialogData> dialogData;
};
