#pragma once
#include "CoreMinimal.h"
#include "DialogueData.h"
#include "Components/Button.h"
#include "GameLogic/UI/DialogUI.h"
#include "UObject/Object.h"
#include "DialogManager.generated.h"

class AMyCPPCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndDialogDelegate);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJET_MORPHEE_API UDialogManager : public UActorComponent
{
	GENERATED_BODY()
private:
	
	UPROPERTY()
	UDialogUI* dialogUI;
	
	UPROPERTY()
	UButton* skipButton;
	
	UPROPERTY()
	APlayerController* playerController;
	
	UPROPERTY()
	int CurrentLineIndex = 0;
	UPROPERTY()
	FString CurrentLeftCharacterID = "None";
	UPROPERTY()
	FString CurrentRightCharacterID = "None";
	UPROPERTY()
	TArray<UDialogueLine*> CurrentDialogLines;
	
	UFUNCTION()
	bool TryInitialize();
	UFUNCTION()
	bool TrySetUIOnlyInputMode(bool isUIOnly);
	UFUNCTION()
	void ActivateSkipButton();
	
	UFUNCTION()
	static bool InvalidError(const FString& invalidProperty);
	UFUNCTION()
	void PlayLine(UDialogueLine* line);
	
	UFUNCTION()
	void TriggerLineEnd();
	UFUNCTION()
	void EndPlayDialog();
	
public:
	UDialogManager();
	
	UPROPERTY()
	AMyCPPCharacter* PlayerCharacter;
	
	UFUNCTION()
	void StartPlayDialog(FString leftCharacter, FString rightCharacter, TArray<UDialogueLine*> dialog);
	
	FEndDialogDelegate OnEndDialog;
	
};
