#include "FlowGraph/Nodes/DialogueBubble_FlowNode.h"


void UDialogueBubble_FlowNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GetDataTable();
	if (isCharacterValid && CharactersInfo != nullptr)
	{
		currentCharacterInfo = CharactersInfo->FindRow<FCharacterInfo>(*Character, "");
		if (currentCharacterInfo != nullptr)
		{
			Name = currentCharacterInfo->Name;
			if (isExpressionValid)
			{
				ECharacterExpression* expressionEnum = ExpressionOptions.Find(Expression);
				if (expressionEnum != nullptr)
				{
					TSoftObjectPtr<UTexture2D>* portrait = currentCharacterInfo->Portraits.Find(*expressionEnum);
					if (portrait != nullptr)
					{
						Portrait = *portrait;
					}
				}
			}
		}
	}
}



TWeakObjectPtr<UDataTable> UDialogueBubble_FlowNode::GetDataTable()
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

TArray<FString> UDialogueBubble_FlowNode::GetInitialCharacterOptions()
{
	if (GetDataTable() == nullptr)
	{
		isCharacterValid = false;
		return {"DataTableNotLinked"};
	}
	TArray<FString> options;
	for (FName name : GetDataTable()->GetRowNames())
	{
		options.Add(name.ToString());
	}
	isCharacterValid = true;
	return options;
}

TArray<FString> UDialogueBubble_FlowNode::GetInitialExpressionOptions()
{	
	if (isCharacterValid && currentCharacterInfo != nullptr)
	{
		TArray<FString> options;
		
		for (ECharacterExpression expression : TEnumRange<ECharacterExpression>())
		{
			if (!currentCharacterInfo->Portraits.Find(expression)->IsNull())
			{
				FString o = *UEnum::GetValueAsName(expression).ToString();
				options.Add(o);
				ExpressionOptions.Add(o);
				ExpressionOptions[o] = expression;
			}
		}
		isExpressionValid = true;
		return options;
	}
	isExpressionValid = false;
	return {"NotValid"};
}

FCharacterInfo::FCharacterInfo()
{
	for (ECharacterExpression expression : TEnumRange<ECharacterExpression>())
	{
		Portraits.Add(expression);
	}
}
