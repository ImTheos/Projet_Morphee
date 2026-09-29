#include "GameLogic/Dialogue/DialogueData.h"


UDialogueLine::UDialogueLine() : isExpressionValid(false), mainCharacterInfo(nullptr){}


UDialogueLine* UDialogueLine::Create(UObject* Outer, FName TableId, FString& Key, FString& CharacterID, FString& OtherCharacterID)
{
	UDialogueLine* NewLine = NewObject<UDialogueLine>(Outer);
	NewLine->Line = FText::FromStringTable(TableId, Key);
	NewLine->MainCharacterID = CharacterID;
	NewLine->OtherCharacterID = OtherCharacterID;
	NewLine->isExpressionValid = false; 
	NewLine->isOtherExpressionValid = false; 
	
	TArray<FString> expressions = NewLine->GetMainCharacterExpressionOptions();
	if (!expressions.IsEmpty())
		NewLine->Expression = expressions[0];
	
	TArray<FString> otherExpressions = NewLine->GetOtherCharacterExpressionOptions();
	if (!otherExpressions.IsEmpty())
		NewLine->OtherCharacterExpression = otherExpressions[0];
	
	NewLine->UpdateCharacterData();	
	return NewLine;
}

void UDialogueLine::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateCharacterData();
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}

TArray<FString> UDialogueLine::GetMainCharacterExpressionOptions()
{	
	if (TryUpdateCharacterInfo())
	{
		TArray<FString> options;
		
		GetAvailableExpressionOptionsFromMain();
		for (const auto& e : MainExpressionOptions)
		{
			options.Add(e.Key);
		}

		isExpressionValid = true;
		return options;
	}
	isExpressionValid = false;
	return {"NotValid"};
}

TArray<FString> UDialogueLine::GetOtherCharacterExpressionOptions()
{
	if (TryUpdateCharacterInfo())
	{
		TArray<FString> options;
		
		GetAvailableExpressionOptionsFromOther();
		for (const auto& e : otherExpressionOptions)
		{
			options.Add(e.Key);
		}

		isOtherExpressionValid = true;
		return options;
	}
	isOtherExpressionValid = false;
	return {"NotValid"};
}

void UDialogueLine::UpdateCharacterData()
{
	if (isExpressionValid)
		Portrait = ResolvePortraitForCurrentExpression(true);
	if (otherCharacterInfo != nullptr)
		OtherPortrait = ResolvePortraitForCurrentExpression(false);
}

TSoftObjectPtr<UTexture2D> UDialogueLine::ResolvePortraitForCurrentExpression(bool isMainCharacter)
{
	if (!TryUpdateCharacterInfo()) return nullptr;

	ECharacterExpression* expressionEnum;
	TSoftObjectPtr<UTexture2D>* portrait = nullptr;
	
	if (isMainCharacter)
	{
		GetAvailableExpressionOptionsFromMain();
		expressionEnum = MainExpressionOptions.Find(Expression);
		
		if (expressionEnum == nullptr)
		{
			if (MainExpressionOptions.begin() == MainExpressionOptions.end()) return nullptr;
			Expression = MainExpressionOptions.begin()->Key;
			expressionEnum = &MainExpressionOptions.begin()->Value;
		}

		portrait = mainCharacterInfo->Portraits.Find(*expressionEnum);		
	}	
	else
	{
		GetAvailableExpressionOptionsFromOther();
		expressionEnum = otherExpressionOptions.Find(OtherCharacterExpression);
		
		if (expressionEnum == nullptr)
		{
			if (otherExpressionOptions.begin() == otherExpressionOptions.end()) return nullptr;
			OtherCharacterExpression = otherExpressionOptions.begin()->Key;
			expressionEnum = &otherExpressionOptions.begin()->Value;
		}

		portrait = otherCharacterInfo->Portraits.Find(*expressionEnum);	
	}
	
	
	return portrait ? *portrait : nullptr;
}

bool UDialogueLine::TryUpdateCharacterInfo()
{
	mainCharacterInfo = UDialogHelper::TryGetCharacterInfo(*MainCharacterID);
	
	if (mainCharacterInfo == nullptr) return false;
	
	Name = mainCharacterInfo->Name;
	DebugColor = mainCharacterInfo->DebugColor;
	
	otherCharacterInfo = UDialogHelper::TryGetCharacterInfo(*OtherCharacterID);
	
	return true;
}

void UDialogueLine::GetAvailableExpressionOptionsFromMain()
{
	MainExpressionOptions.Empty();
	for(auto e : UDialogHelper::GetAvailableExpressionOptionsFromCharacter(MainCharacterID))
	{
		FString o = *UEnum::GetValueAsName(e).ToString();
		MainExpressionOptions.Add(o, e);
	}
}

void UDialogueLine::GetAvailableExpressionOptionsFromOther()
{
	otherExpressionOptions.Empty();
	for(auto e : UDialogHelper::GetAvailableExpressionOptionsFromCharacter(OtherCharacterID))
	{
		FString o = *UEnum::GetValueAsName(e).ToString();
		otherExpressionOptions.Add(o, e);
	}
}

TWeakObjectPtr<UDataTable> UDialogHelper::GetCharactersDatatable()
{
	if (CharactersInfo == nullptr)
	{
		FSoftObjectPath path(TEXT("DataTable'/Game/Data/Narration/DT_Characters'"));
		CharactersInfo = Cast<UDataTable>(path.ResolveObject());
		if (CharactersInfo == nullptr)
		{
			CharactersInfo = CastChecked<UDataTable>(path.TryLoad());
		}  
	}
	return CharactersInfo;
}

// DIALOG HELPER

TWeakObjectPtr<UDataTable> UDialogHelper::CharactersInfo;

FCharacterInfo* UDialogHelper::TryGetCharacterInfo(const FString& CharacterID)
{
	if (GetCharactersDatatable() == nullptr) return nullptr;
	return GetCharactersDatatable()->FindRow<FCharacterInfo>(*CharacterID, "");
}

TArray<ECharacterExpression> UDialogHelper::GetAvailableExpressionOptionsFromCharacter(const FString& CharacterID)
{
	if (CharactersInfo == nullptr) return TArray<ECharacterExpression>();
	
	TArray<ECharacterExpression> result;
	auto info = GetCharactersDatatable()->FindRow<FCharacterInfo>(*CharacterID, "");
	
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
