#include "PlayFreeDialog.h"

#include "MyCPPCharacter.h"
#include "GameLogic/UI/PlayerUI.h"
#include "Kismet/GameplayStatics.h"

UPlayFreeDialog::UPlayFreeDialog(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
	OutputPins = {FFlowPin(TEXT(" ")), FFlowPin(TEXT("Completed"))};
}


void UPlayFreeDialog::ExecuteInput(const FName& PinName)
{
	auto playerCharacter = Cast<AMyCPPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (playerCharacter == nullptr) return;
	if (playerCharacter->playerUIWidget == nullptr) return;
	
	PlayerUI = playerCharacter->playerUIWidget;
	PlayerUI->PlayFreeDialog(text, duration);
	PlayerUI->OnEndPlayFreeDialogDelegate.AddDynamic(this, &UPlayFreeDialog::Complete);
	
	TriggerOutput(TEXT(" "), false);	
	Super::ExecuteInput(PinName);
}

void UPlayFreeDialog::Complete()
{
	PlayerUI->OnEndPlayFreeDialogDelegate.RemoveAll(this);
	TriggerOutput(TEXT("Completed"), true);	
}

