#include "GameLogic/Dialogue/FlowGraph/Nodes/PlayDialog.h"

#include "DetailCategoryBuilder.h"
#include "MyCPPCharacter.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEd.h"

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
		TryGetLines();
		OldLevel = Level;
	}
	if (OldDialog != Dialog)
	{
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

int UPlayDialog::FindLine(const TArray<UDialogueLine*>& allLines, const FString& characterID, FText line)
{
	for (int i = 0; i < allLines.Num(); i++)
	{
		if (allLines[i]->Main->ID == characterID && allLines[i]->Line.EqualTo(line))
			return i;
	}
	return -1;
}

void UPlayDialog::TryGetLines()
{
	if (Level.IsEmpty() || !TextDatabase.Contains(Level))
	{
		Lines.Empty();
		return;
	}
	
	auto oldLines = Lines;
	Lines.Empty();
	FStringTableConstRef currentStringTable = TextDatabase[Level]->GetStringTable();
	
	TArray<FString> Options;

	currentStringTable->EnumerateKeysAndSourceStrings([this, oldLines](const FTextKey& Key, const FString& SourceString) -> bool
	{
		TArray<FString> Parsed;
		FString KeyString(Key.GetChars());
		KeyString.ParseIntoArray(Parsed, TEXT("_"), false);
		FString& characterID = Parsed[1];
		if (Parsed[0] == Dialog && !characterID.IsEmpty())
		{
			int oldLineIndex = FindLine(oldLines, characterID, FText::FromStringTable(TextDatabase[Level]->GetStringTableId(), KeyString));
			if (oldLineIndex == -1)
			{
				Lines.Add(UDialogueLine::Create(this, TextDatabase[Level]->GetStringTableId(), KeyString, characterID));
			}
			else
			{
				Lines.Add(oldLines[oldLineIndex]);				
			}
		}
		return true;
	});
	SetOtherCharacters();
}

void UPlayDialog::SetOtherCharacters()
{
	TArray<FString> charactersInDialog = GetCharacterOptions();
	for (UDialogueLine* l : Lines)
	{

		int oldIndex = -1;
		
		if (!l->Others.IsEmpty())
		{
			TArray<UCharacterDialogInfo*> oldOthers = l->Others;
			l->Others.Empty();
			
			for (FString& id : charactersInDialog)
			{
				if (id == l->Main->ID) continue;
				
				for (int i = 0; i < oldOthers.Num(); i++)
				{
					if (oldOthers[i]->ID == id)
						oldIndex = i;
				}				
				
				if (oldIndex == -1)
				{
					l->Others.Add(UCharacterDialogInfo::Create(this, id));
				}
				else
				{
					l->Others.Add(oldOthers[oldIndex]);
				}
			}
		}
		else
		{
			for (FString& id : charactersInDialog)
			{
				if (id == l->Main->ID) continue;
				l->Others.Add(UCharacterDialogInfo::Create(this, id));
			}
		}
	}
}

EDataValidationResult UPlayDialog::ValidateNode()
{
	TryGetLines();
	OldLevel = Level;
	OldDialog = Dialog;
	return Super::ValidateNode();
}


void UPlayDialog::Finish()
{
	if (IsValid(PlayerCharacter) && IsValid(PlayerCharacter->DialogManager))
	{
		PlayerCharacter->DialogManager->OnEndDialog.RemoveDynamic(this, &UPlayDialog::EndDialog);
	}
	Super::Finish();
}

FCharacterInfo::FCharacterInfo()
{
	for (ECharacterExpression expression : TEnumRange<ECharacterExpression>())
	{
		Portraits.Add(expression);
	}
}

