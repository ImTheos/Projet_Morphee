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
	
	UPROPERTY(EditAnywhere, EditFixedSize, meta = (ShowOnlyInnerProperties, EditFixedSize, EditFixedOrder))
	TArray<UDialogueLine*> Lines;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION()
	void EndDialog();
	
	virtual EDataValidationResult ValidateNode() override;
	virtual void Finish() override;
	 
private:
	UPROPERTY()
	TMap<FString, UStringTable*> TextDatabase;
	FString OldLevel;
	FString OldDialog;
	
	UPROPERTY()
	AMyCPPCharacter* PlayerCharacter;
	
	UFUNCTION()	
	TArray<FString> GetLevelOptions();
	UFUNCTION()
	TArray<FString> GetDialogOptions();
	UFUNCTION()
	TArray<FString> GetCharacterOptions();
	
	UFUNCTION()
	static int FindLine(const TArray<UDialogueLine*>& allLines, const FString& characterID, FText line);
	
	UFUNCTION()
	void TryGetLines();
	void SetOtherCharacters();
};


