#include "GameLogic/Dialogue/DialogManager.h"
#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"
#include "Kismet/GameplayStatics.h"



UDialogManager::UDialogManager()
{
	
}

bool UDialogManager::TryInitialize()
{
	if (!PlayerCharacter)
		PlayerCharacter = Cast<AMyCPPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
		return InvalidError(("PlayerCharacter"));

	if (!playerController)
		playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!IsValid(playerController))
		return InvalidError(("PlayerController"));

	const UPlayerUI* playerUI = PlayerCharacter->playerUIWidget;
	
	if (!IsValid(playerUI))
		return InvalidError(("PlayerUI"));
	
	if (!dialogUI)
		dialogUI = playerUI->dialogUI;

	if (!IsValid(dialogUI))
		return InvalidError(("Dialog UI"));


	return true;
}

bool UDialogManager::TrySetUIOnlyInputMode(bool isUIOnly)
{
	if (TryInitialize())
	{
		if (isUIOnly)
			playerController->SetInputMode(FInputModeUIOnly());
		else
			playerController->SetInputMode(FInputModeGameAndUI());

		playerController->FlushPressedKeys();
		return true;
	}
	return false;
}

void UDialogManager::StartPlayDialog(FString leftCharacter, FString rightCharacter, TArray<UDialogueLine*> dialog)
{
	if (!TrySetUIOnlyInputMode(true)) return;
	
	dialogUI->SetVisibility(ESlateVisibility::Visible);
	
	CurrentLineIndex = 0;	
	CurrentLeftCharacterID = leftCharacter;
	CurrentRightCharacterID = rightCharacter;	
	CurrentDialogLines = dialog;
	
	PlayLine(CurrentDialogLines[CurrentLineIndex]);
}

void UDialogManager::PlayLine(UDialogueLine* line)
{
	TryInitialize();

	if (!IsValid(dialogUI))
	{
		UE_LOG(LogTemp, Error, TEXT("dialogUI is not valid"));
		////////////// TriggerFirstOutput(true);
		return;
	}
	skipButton = dialogUI->skipButton;
	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		/////////////  TriggerFirstOutput(true);
		return;
	}
	
	skipButton->SetVisibility(ESlateVisibility::Hidden);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Hidden);
	
	dialogUI->displaySkipButtonDelegate.AddUniqueDynamic(this, &UDialogManager::ActivateSkipButton);

	if (line->bAnimateText)
		dialogUI->SetText(line->Line, line->Name, line->LetterDelay);
	else
		dialogUI->SetTextNoDelay(line->Line, line->Name);
	
	// change characters icons
	UTexture2D* leftPortrait = nullptr;
	UTexture2D* rightPortrait = nullptr;
	
	if (CurrentLeftCharacterID != "None")
	{
		const TSoftObjectPtr<UTexture2D>& src =
			CurrentLeftCharacterID == line->MainCharacterID ? line->Portrait : line->OtherPortrait;
		leftPortrait = src.LoadSynchronous();
	}
	if (CurrentRightCharacterID != "None")
	{
		const TSoftObjectPtr<UTexture2D>& src =
			CurrentRightCharacterID == line->MainCharacterID ? line->Portrait : line->OtherPortrait;
		rightPortrait = src.LoadSynchronous();
	}
	// if (CurrentLeftCharacterID != "None")
	// 	leftPortrait = CurrentLeftCharacterID == line->MainCharacterID ? line->Portrait.LoadSynchronous() : line->OtherPortrait.LoadSynchronous();
	// if (CurrentRightCharacterID != "None")
	// 	rightPortrait = CurrentRightCharacterID == line->MainCharacterID ? line->Portrait.LoadSynchronous() : line->OtherPortrait.LoadSynchronous();
	//
	dialogUI->setImages(leftPortrait, rightPortrait);
}


void UDialogManager::ActivateSkipButton()
{
	dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDialogManager::ActivateSkipButton);

	skipButton = dialogUI->GetSkipButton();

	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		return;
	}
	
	skipButton->OnClicked.AddUniqueDynamic(this, &UDialogManager::TriggerLineEnd);

	skipButton->SetVisibility(ESlateVisibility::Visible);
	dialogUI->skipLogo->SetVisibility(ESlateVisibility::Visible);
}

void UDialogManager::TriggerLineEnd()
{
	if (!IsValid(skipButton))
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayDialog : skipButton is not valid"));
		///////////////  TriggerFirstOutput(true);
		return;
	}
	
	skipButton->OnClicked.RemoveDynamic(this, &UDialogManager::TriggerLineEnd);
	
	if (CurrentLineIndex < CurrentDialogLines.Num()-1)
	{
		CurrentLineIndex++;
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			PlayLine(CurrentDialogLines[CurrentLineIndex]);
		});
	}
	else
	{
		EndPlayDialog();
	}
	
	
	/////////////////  TriggerFirstOutput(true);
}

void UDialogManager::EndPlayDialog()
{	
	dialogUI->SetVisibility(ESlateVisibility::Hidden);
 	TrySetUIOnlyInputMode(false);	
	dialogUI->displaySkipButtonDelegate.RemoveDynamic(this, &UDialogManager::ActivateSkipButton);
	dialogUI->skipButton->OnClicked.RemoveDynamic(this, &UDialogManager::TriggerLineEnd);
	OnEndDialog.Broadcast();
}


bool UDialogManager::InvalidError(const FString& invalidProperty)
{
	UE_LOG(LogTemp, Error, TEXT("Dialog : invalid %s"), *invalidProperty);
	return false;
}

