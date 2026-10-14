#include "GameLogic/Dialogue/DialogueData.h"


//////////////////////// DIALOGUE LINE ////////////////////////

UDialogueLine::UDialogueLine(){}

UDialogueLine* UDialogueLine::Create(UObject* Outer, FName TableId, FString& Key, FString& CharacterID)
{
	UDialogueLine* NewLine = NewObject<UDialogueLine>(Outer);
	NewLine->Line = FText::FromStringTable(TableId, Key);
	NewLine->Main = UCharacterDialogInfo::Create(Outer, TableId.ToString());
	NewLine->Main->ID = CharacterID;

	return NewLine;
}


//////////////////////// CHARACTER DIALOGUE INFO ////////////////////////

UCharacterDialogInfo* UCharacterDialogInfo::Create(UObject* outer,const FString& id)
{
	UCharacterDialogInfo* newInfo = NewObject<UCharacterDialogInfo>(outer);
	
	newInfo->ID = id;
	TArray<FString> expressions = newInfo->GetExpressionOptions();
	if (!expressions.IsEmpty())
		newInfo->Expression = expressions[0];
	
	newInfo->Portrait = newInfo->ResolvePortraitForCurrentExpression();
	
	return newInfo;
}

void UCharacterDialogInfo::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateCharacterData();
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}

TArray<FString> UCharacterDialogInfo::GetExpressionOptions()
{	
	if (TryUpdateCharacterInfo())
	{
		TArray<FString> options;
		
		for (const auto& e : MainExpressionOptions)
		{
			options.Add(e.Key);
		}
		return options;
	}
	return {"NotValid"};
}

void UCharacterDialogInfo::UpdateCharacterData()
{
	Portrait = ResolvePortraitForCurrentExpression();
}

TSoftObjectPtr<UTexture2D> UCharacterDialogInfo::ResolvePortraitForCurrentExpression()
{
	if (!TryUpdateCharacterInfo()) return nullptr;
	
	GetAvailableExpressionOptions();
	ECharacterExpression* expressionEnum = MainExpressionOptions.Find(Expression);
	
	if (expressionEnum == nullptr)
	{
		if (MainExpressionOptions.begin() == MainExpressionOptions.end()) return nullptr;
		Expression = MainExpressionOptions.begin()->Key;
		expressionEnum = &MainExpressionOptions.begin()->Value;
	}

	TSoftObjectPtr<UTexture2D>* portrait = CharacterInfo->Portraits.Find(*expressionEnum);		

	return portrait ? *portrait : nullptr;
}

bool UCharacterDialogInfo::TryUpdateCharacterInfo()
{
	CharacterInfo = UDialogHelper::TryGetCharacterInfo(ID);
	
	if (CharacterInfo == nullptr) return false;
	
	Name = CharacterInfo->Name;
	DebugColor = CharacterInfo->DebugColor;
	
	return true;
}

void UCharacterDialogInfo::GetAvailableExpressionOptions()
{
	MainExpressionOptions.Empty();
	for(auto e : UDialogHelper::GetAvailableExpressionOptionsFromCharacter(ID))
	{
		FString o = *UEnum::GetValueAsName(e).ToString();
		MainExpressionOptions.Add(o, e);
	}
}


//////////////////////// DIALOG HELPER ////////////////////////

TWeakObjectPtr<UDataTable> UDialogHelper::CharactersDatabase;

FCharacterInfo* UDialogHelper::TryGetCharacterInfo(const FString& CharacterID)
{
	if (GetCharactersDatatable() == nullptr) return nullptr;
	auto v = CharactersDatabase->FindRow<FCharacterInfo>(*CharacterID, "");
	return v;
}

TArray<ECharacterExpression> UDialogHelper::GetAvailableExpressionOptionsFromCharacter(const FString& CharacterID)
{
	if (GetCharactersDatatable()  == nullptr) return TArray<ECharacterExpression>();
	
	TArray<ECharacterExpression> result;
	auto info = CharactersDatabase->FindRow<FCharacterInfo>(*CharacterID, "");
	
	if (info==nullptr) return result;
	
	for (ECharacterExpression expression : TEnumRange<ECharacterExpression>())
	{
		if (!info->Portraits.Find(expression)->IsNull())
		{
			result.Add(expression);
		}
	}
	return result;
}

TWeakObjectPtr<UDataTable> UDialogHelper::GetCharactersDatatable()
{
	if (CharactersDatabase == nullptr)
	{
		FSoftObjectPath path(TEXT("DataTable'/Game/Data/Narration/DT_Characters'"));
		CharactersDatabase = Cast<UDataTable>(path.ResolveObject());
		if (CharactersDatabase == nullptr)
		{
			CharactersDatabase = CastChecked<UDataTable>(path.TryLoad());
		}  
	}
	return CharactersDatabase;
}
