#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "UObject/Object.h"

class FDetailArrayBuilder;

class PROJET_MORPHEE_EDITOR_API FDialogueDetailsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FDialogueDetailsCustomization);
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	IDetailChildrenBuilder& AdditionalDialogInfoUI(IDetailLayoutBuilder& DetailBuilder);
	TArray<TSharedPtr<FSlateBrush>> PortraitBrushes;
	
	TSharedRef<FDetailArrayBuilder> LinesUI(IDetailLayoutBuilder& DetailBuilder, FSimpleDelegate& OnValueChanged);
};

