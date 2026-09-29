#include "GameLogic/Dialogue/FlowGraph/Nodes/PlayDialog.h"

#include "DetailCategoryBuilder.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

UPlayDialog::UPlayDialog(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
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

TArray<FString> UPlayDialog::GetInitialCharacterOptions(bool IsLeft)
{
	if (Level.IsEmpty() || Dialog.IsEmpty() || Lines.IsEmpty()|| !TextDatabase.Contains(Level)) return {"Error - Dialog not found"};
	TArray<FString> Options;
	
	Options.Add("None");
	
	FString& otherCharacter = IsLeft? RightCharacter : LeftCharacter;
	
	for (UDialogueLine* l : Lines)
	{
		if (!Options.Contains(l->MainCharacterID) && l->MainCharacterID != otherCharacter)
		{
			Options.Add(l->MainCharacterID);		
		}
	}
	
	return Options;
}

TArray<FString> UPlayDialog::GetCharacterOptions()
{
	if (Level.IsEmpty() || Dialog.IsEmpty() || Lines.IsEmpty() || !TextDatabase.Contains(Level)) return {"Error - Dialog not found"};
	TArray<FString> Options;
	
	for (UDialogueLine* l : Lines)
	{
		if (!Options.Contains(l->MainCharacterID))
			Options.Add(l->MainCharacterID);		
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
			Lines.Add(UDialogueLine::Create(this, TextDatabase[Level]->GetStringTableId(), KeyString, characterID, otherCharacter));
		}
		return true;
	});
	SetOtherCharacter();
}

void UPlayDialog::SetOtherCharacter()
{
	TArray<FString> charactersInDialog = GetCharacterOptions();
	if (charactersInDialog.Num() == 2)
	{
		for (UDialogueLine* l : Lines)
		{
			l->OtherCharacterID = charactersInDialog[0] == l->MainCharacterID ? charactersInDialog[1] : charactersInDialog[0];
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

