// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/UI/DialogUI.h"

#include "Blueprint/WidgetTree.h"

void UDialogUI::InitTextBlock()
{
	TArray<UWidget*> widgetArray;
	WidgetTree->GetAllWidgets(widgetArray);

	// Loop through all UI widgets to find the dialog UI
	for (UWidget* widget : widgetArray)
	{
		if (URichTextBlock* tmpDialogTextBlock = Cast<URichTextBlock>(widget))
		{
			dialogTextBlock = tmpDialogTextBlock;
			break;
		}
	}
}

void UDialogUI::SetText(const FText& dialogText)
{
	if (!dialogTextBlock)
	{
		InitTextBlock();
	}

	if (!dialogTextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerUI : no dialog text block available"));
		return;
	}

	dialogTextBlock->SetText(dialogText);
}
