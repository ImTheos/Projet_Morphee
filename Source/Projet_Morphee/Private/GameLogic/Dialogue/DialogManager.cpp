// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Dialogue/DialogManager.h"

#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"
#include "Kismet/GameplayStatics.h"
//
//
//
// bool UDialogManager::TryInitialize()
// {
// 	playerCharacter = Cast<AMyCPPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
// 	
// 	if (!IsValid(playerCharacter))
// 		return InvalidError(("PlayerCharacter"));
//
// 	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
// 	const UPlayerUI* playerUI = playerCharacter->playerUIWidget;
//
// 	if (!IsValid(playerController))
// 		return InvalidError(("PlayerController"));
//
// 	if (!IsValid(playerUI))
// 		return InvalidError(("PlayerUI"));
// 	
// 	dialogUI = playerUI->dialogUI;
//
// 	if (!IsValid(dialogUI))
// 		return InvalidError(("Dialog UI"));
//
// 	return true;
// }
//
// bool UDialogManager::TrySetUIOnlyInputMode(bool isUIOnly)
// {
// 	if (TryInitialize())
// 	{
// 		if (isUIOnly)
// 			playerController->SetInputMode(FInputModeUIOnly());
// 		else
// 			playerController->SetInputMode(FInputModeGameAndUI());
//
// 		playerController->FlushPressedKeys();
// 		return true;
// 	}
// 	return false;
// }
//
//
//
// bool UDialogManager::InvalidError(const FString& invalidProperty)
// {
// 	UE_LOG(LogTemp, Error, TEXT("Dialog : invalid %s"), *invalidProperty);
// 	return false;
// }
//
//
// void UDialogManager::PlayDialog(TArray<UDialogueLine*> dialog)
// {
// 	if (!TrySetUIOnlyInputMode(true)) return;
// 	
// 	dialogUI->SetVisibility(ESlateVisibility::Visible);
// 	
// 	for (UDialogueLine* line : dialog)
// 	{
// 		
// 	}
// 	
// 	
// 	dialogUI->SetVisibility(ESlateVisibility::Hidden);
// 	TrySetUIOnlyInputMode(false);	
// }
//
// void UDialogManager::ActivateSkipButton()
// {
// 	dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDialogManager::ActivateSkipButton);
// 	
// 	skipButton = dialogUI->GetSkipButton();
//
// 	if (!IsValid(skipButton))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
// 		return;
// 	}
// 	
// 	skipButton->OnClicked.AddDynamic(this, &UDialogManager::TriggerEnd);
//
// 	// Shows button
// 	skipButton->SetVisibility(ESlateVisibility::Visible);
// 	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Visible);
// }
//
// void UDialogManager::TriggerEnd()
// {
// 	if (!IsValid(skipButton))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
// 		///////////////  TriggerFirstOutput(true);
// 		return;
// 	}
// 	
// 	skipButton->OnClicked.RemoveDynamic(this, &UDialogManager::TriggerEnd);
// 	
// 	UE_LOG(LogTemp, Display, TEXT("Trigger End reached"));
// 	/////////////////  TriggerFirstOutput(true);
// }
//
// void UDialogManager::PlayLine(UDialogueLine* line)
// {
// 	TryInitialize();
//
// 	if (!IsValid(dialogUI))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("dialogUI is not valid"));
// 		////////////// TriggerFirstOutput(true);
// 		return;
// 	}
//
// 	skipButton = dialogUI->skipButton;
// 	if (!IsValid(skipButton))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
// 		/////////////  TriggerFirstOutput(true);
// 		return;
// 	}
// 	
// 	// Hides button
// 	skipButton->SetVisibility(ESlateVisibility::Hidden);
// 	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Hidden);
// 	
// 	// binds skip button activation, used to display the skip button when the whole text will be displayed
// 	dialogUI->displaySkipButtonDelegate.AddDynamic(this, &UDisplayDialog::ActivateSkipButton);
//
// 	// Change dialog text and character name, animates the text apperance if this feature is activated
// 	if (bDelayTextAppearance)
// 	{
// 		dialogUI->SetText(line->Line, line->Name, textDelayTime);
// 	}
// 	else
// 	{
// 		dialogUI->SetTextNoDelay(line->Line, line->Name);
// 	}
// 	
// 	// change characters icons
// 	dialogUI->setImages(leftImage, rightImage);
// }
