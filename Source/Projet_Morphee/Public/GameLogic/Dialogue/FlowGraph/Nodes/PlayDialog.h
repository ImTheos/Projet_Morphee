#pragma once
#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "Engine/DataTable.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "GameLogic/Dialogue/DialogManager.h"
#include "Internationalization/StringTable.h"
#include "GameLogic/Dialogue/DialogueData.h"
#include "PlayDialog.generated.h"


UCLASS(DontCollapseCategories)
class PROJET_MORPHEE_API UPlayDialog : public UFlowNode
{
	GENERATED_UCLASS_BODY()
	
	virtual void ExecuteInput(const FName& PinName) override;
	
public:
	
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetLevelOptions"))
	FString Level;	
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetDialogOptions"))
	FString Dialog;
	
	UPROPERTY(EditAnywhere, Category="AdditionalInfo", meta = (GetOptions = "GetLeftCharacterOptions"))
	FString LeftCharacter = "None";
	UPROPERTY(EditAnywhere, Category="AdditionalInfo", meta = (GetOptions = "GetRightCharacterOptions"))
	FString RightCharacter = "None";
	
	UPROPERTY(EditAnywhere, EditFixedSize, meta = (ShowOnlyInnerProperties, EditFixedSize, EditFixedOrder))
	TArray<UDialogueLine*> Lines;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION()
	void EndDialog();
	
	
private:
	TMap<FString, UStringTable*> TextDatabase;
	FString OldLevel;
	FString OldDialog;
	
	AMyCPPCharacter* PlayerCharacter;
	
	UFUNCTION()	
	TArray<FString> GetLevelOptions();
	UFUNCTION()
	TArray<FString> GetDialogOptions();
	UFUNCTION()
	TArray<FString> GetCharacterOptions();
	UFUNCTION()
	TArray<FString> GetInitialCharacterOptions(bool IsLeft);
	UFUNCTION()
	TArray<FString> GetLeftCharacterOptions(){ return GetInitialCharacterOptions(true); }
	UFUNCTION()
	TArray<FString> GetRightCharacterOptions(){ return GetInitialCharacterOptions(false); }
	UFUNCTION()
	void TryGetLines();
	void SetOtherCharacter();
};


