// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayDialogv2.h"

#include "FlowSubsystem.h"
#include "MyCPPCharacter.h"
#include "Kismet/GameplayStatics.h"

bool UDisplayDialogv2::TrySetInputMode(const FInputModeDataBase& inputMode)
{
	const TSet<TWeakObjectPtr<AMyCPPCharacter>> playerCharacterSet = GetFlowSubsystem()->GetActors<AMyCPPCharacter>(playerTag);

	if (playerCharacterSet.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : no PlayerCharacter has required tag : %s"), *playerTag.ToString());
		return false;
	}

	const AMyCPPCharacter* playerCharacter = playerCharacterSet.Array()[0].Get();

	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : invalid PlayerCharacter"));
		return false;
	}

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateDialog FlowNode : invalid PlayerController"));
		return false;
	}

	playerController->SetInputMode(inputMode);
	playerController->FlushPressedKeys();
	return true;
}

void UDisplayDialogv2::ExecuteInput(const FName& PinName)
{
	if (!TryInitialize() || !TrySetInputMode(FInputModeUIOnly()))
	{
		UE_LOG(LogTemp, Error, TEXT("UDisplayDialogv2 : failed to initialize"));
		TriggerFirstOutput(true);
		return;
	}
	
	if (!IsValid(dialogUI))
	{
		UE_LOG(LogTemp, Error, TEXT("dialogUI is not valid"));
		TriggerFirstOutput(true);
		return;
	}
	
	if (dialogData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : no dialog data"));
		EndDialog();
		return;
	}

	skipButton = dialogUI->skipButton;
	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		TriggerFirstOutput(true);
		return;
	}
	
	// Init : Hide skip button
	skipButton->SetVisibility(ESlateVisibility::Hidden);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Hidden);
	
	dialogUI->SetVisibility(ESlateVisibility::Visible);
	
	// display first dialog
	DisplayNextDialogLine();
}

void UDisplayDialogv2::ActivateSkipButton()
{
	if (!IsValid(dialogUI))
	{
		return;
	}
	dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDisplayDialogv2::ActivateSkipButton);
	
	skipButton = dialogUI->GetSkipButton();

	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		return;
	}
	
	skipButton->OnClicked.AddDynamic(this, &UDisplayDialogv2::TriggerSkip);

	// Shows button
	skipButton->SetVisibility(ESlateVisibility::Visible);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Visible);
}

void UDisplayDialogv2::TriggerSkip()
{
	DisplayNextDialogLine();
}

void UDisplayDialogv2::EndDialog()
{
	if (!TrySetInputMode(FInputModeGameAndUI()))
	{
		UE_LOG(LogTemp, Error, TEXT("UDisplayDialogv2::EndDialog : failed to set input mode"));
		TriggerFirstOutput(true);
		return;
	}
	
	if (IsValid(dialogUI))
	{
		dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDisplayDialogv2::ActivateSkipButton);
		dialogUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	TriggerFirstOutput(true);
}

void UDisplayDialogv2::DisplayNextDialogLine()
{
	if (currentDialogIndex >= dialogData.Num())
	{
		EndDialog();
	}
	
	// Hides button
	skipButton->SetVisibility(ESlateVisibility::Hidden);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Hidden);
	
	// binds skip button activation, used to display the skip button when the whole text will be displayed
	dialogUI->displaySkipButtonDelegate.AddDynamic(this, &UDisplayDialogv2::ActivateSkipButton);

	// Change dialog text and character name, animates the text apperance if this feature is activated
	if (dialogData[currentDialogIndex].bDelayTextAppearance)
	{
		dialogUI->SetText(dialogData[currentDialogIndex].dialogText, dialogData[currentDialogIndex].characterName, dialogData[currentDialogIndex].textDelayTime);
	}
	else
	{
		dialogUI->SetTextNoDelay(dialogData[currentDialogIndex].dialogText, dialogData[currentDialogIndex].characterName);
	}
	
	// change characters icons
	dialogUI->setImages(dialogData[currentDialogIndex].leftImage, dialogData[currentDialogIndex].rightImage);
	
	currentDialogIndex++;
}
