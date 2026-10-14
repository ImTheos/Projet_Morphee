#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "UObject/Object.h"

class IPropertyHandle;
class SBorder;
struct FSlateBrush;
class SScrollBox;
class UCharacterDialogInfo;
class FDetailArrayBuilder;

class PROJET_MORPHEE_EDITOR_API FDialogDetailsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FDialogDetailsCustomization);
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	TArray<TSharedPtr<FSlateBrush>> PortraitBrushes;
	
	TSharedRef<FDetailArrayBuilder> LinesUI(IDetailLayoutBuilder& DetailBuilder, FSimpleDelegate& OnValueChanged);
	TSharedRef<SScrollBox> OthersListUI(FText name, TSharedPtr<IPropertyHandle> OthersHandle, const FSimpleDelegate& OnValueChanged);
	TSharedRef<SBorder> CharacterInfoUI(TSharedRef<IPropertyHandle> Handle, const FSimpleDelegate& OnValueChanged,
	                                    float PortraitSize, FLinearColor backgroundColor);
	
	FVector2D BaseCharacterImageRatio = FVector2D(120.0f, 170.0f);
};

