#include "GameLogic/Dialogue/FlowGraph/Nodes/PlayDialog.h"

#include "DetailCategoryBuilder.h"
#include "MyCPPCharacter.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/GameplayStatics.h"

UPlayDialog::UPlayDialog(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
}

void UPlayDialog::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
	PlayerCharacter = Cast<AMyCPPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter->DialogManager != nullptr)
	{
		PlayerCharacter->DialogManager->StartPlayDialog(Lines);
		PlayerCharacter->DialogManager->OnEndDialog.AddUniqueDynamic(this, &UPlayDialog::EndDialog);
	}
}

void UPlayDialog::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (OldLevel != Level)
	{
		Lines.Empty();
		TryGetLines();
		OldLevel = Level;
	}
	if (OldDialog != Dialog)
	{
		Lines.Empty();
		TryGetLines();
		OldDialog = Dialog;
	}
}

void UPlayDialog::EndDialog()
{
	PlayerCharacter->DialogManager->OnEndDialog.RemoveDynamic(this, &UPlayDialog::EndDialog);
	TriggerFirstOutput(true);
}


TArray<FString> UPlayDialog::GetLevelOptions()
{
	TArray<FString> options;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> StringTableAssets;
	AssetRegistryModule.Get().GetAssetsByClass(UStringTable::StaticClass()->GetClassPathName(), StringTableAssets);
	for (const auto& asset : StringTableAssets)
	{
		FString name = asset.AssetName.ToString();
		UStringTable* StringTableAsset = Cast<UStringTable>(asset.GetAsset());
		if (!StringTableAsset) continue;

		FStringTableConstRef table = StringTableAsset->GetStringTable();
		TextDatabase.Add(name, StringTableAsset);
		options.Add(name);
	}
	return options;
}

TArray<FString> UPlayDialog::GetDialogOptions()
{
	if (Level.IsEmpty() || !TextDatabase.Contains(Level)) return {"Error - Level not found"};
	FStringTableConstRef currentStringTable = TextDatabase[Level]->GetStringTable();
	
	TArray<FString> Options;

	currentStringTable->EnumerateKeysAndSourceStrings([&Options](const FTextKey& Key, const FString& SourceString) -> bool
	{
		TArray<FString> Parsed;
		FString KeyString(Key.GetChars());
		KeyString.ParseIntoArray(Parsed, TEXT("_"), false);
		if (!Options.Contains(Parsed[0]))
		{
			Options.Add(Parsed[0]);		
		}
		return true;
	});
	
	return Options;
}

TArray<FString> UPlayDialog::GetCharacterOptions()
{
	if (Level.IsEmpty() || Dialog.IsEmpty() || Lines.IsEmpty() || !TextDatabase.Contains(Level)) return {"Error - Dialog not found"};
	TArray<FString> Options;
	
	for (UDialogueLine* l : Lines)
	{
		if (!Options.Contains(l->Main->ID))
			Options.Add(l->Main->ID);		
	}
	return Options;
}
void UPlayDialog::TryGetLines()
{
	if (Level.IsEmpty() || !TextDatabase.Contains(Level))
	{
		Lines.Empty();
		return;
	}
	FStringTableConstRef currentStringTable = TextDatabase[Level]->GetStringTable();
	
	TArray<FString> Options;

	currentStringTable->EnumerateKeysAndSourceStrings([this](const FTextKey& Key, const FString& SourceString) -> bool
	{
		TArray<FString> Parsed;
		FString KeyString(Key.GetChars());
		KeyString.ParseIntoArray(Parsed, TEXT("_"), false);
		FString& characterID = Parsed[1];
		if (Parsed[0] == Dialog && !characterID.IsEmpty())
		{
			FString otherCharacter = "None"; 
			Lines.Add(UDialogueLine::Create(this, TextDatabase[Level]->GetStringTableId(), KeyString, characterID));
		}
		return true;
	});
	SetOtherCharacters();
}

void UPlayDialog::SetOtherCharacters()
{
	TArray<FString> charactersInDialog = GetCharacterOptions();
	for (FString& id : charactersInDialog)
	{
		for (UDialogueLine* l : Lines)
		{
			if (id == l->Main->ID) continue;
			l->Others.Add(UCharacterDialogInfo::Create(this, id));
		}
	}
}



FCharacterInfo::FCharacterInfo()
{
	for (ECharacterExpression expression : TEnumRange<ECharacterExpression>())
	{
		Portraits.Add(expression);
	}
}

