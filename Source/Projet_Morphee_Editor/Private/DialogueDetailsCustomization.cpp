#pragma once

#include "DialogueDetailsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "GameLogic/Dialogue/FlowGraph/Nodes/PlayDialog.h"

void FDialogueDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	PortraitBrushes.Empty(); 

	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Play Dialogue Flow Node");
	TSharedRef<IPropertyHandle> LevelProp = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UPlayDialog, Level));
	TSharedRef<IPropertyHandle> DialogueProp = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UPlayDialog, Dialog));
	DetailBuilder.HideProperty(LevelProp);
	DetailBuilder.HideProperty(DialogueProp);
	DetailBuilder.HideCategory("AdditionalInfo");
	
	TSharedRef<IPropertyHandle> LeftCharacter = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UPlayDialog, LeftCharacter));
	TSharedRef<SWidget> LeftCharacterWidget = LeftCharacter->IsValidHandle()? LeftCharacter->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid LeftCharacter"));
	
	TSharedRef<IPropertyHandle> RightCharacter = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UPlayDialog, RightCharacter));
	TSharedRef<SWidget> RightCharacterWidget = RightCharacter->IsValidHandle()? RightCharacter->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid RightCharacter"));
	
	TSharedPtr<IPropertyUtilities> PropUtils = DetailBuilder.GetPropertyUtilities();
	FSimpleDelegate OnValueChanged = FSimpleDelegate::CreateLambda([PropUtils]()
	{
		if (!PropUtils.IsValid()) return;

		TWeakPtr<IPropertyUtilities> WeakUtils = PropUtils;

		TSharedRef<SWidget> DummyAnchor = SNullWidget::NullWidget;
		FSlateApplication::Get().GetRenderer();

		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
			[WeakUtils](float DeltaTime) -> bool
		{
			if (WeakUtils.IsValid())
			{
				WeakUtils.Pin()->ForceRefresh();
			}
			return false; 
		}));
	});

	if (LeftCharacter->IsValidHandle())
		LeftCharacter->SetOnPropertyValueChanged(OnValueChanged);
	if (RightCharacter->IsValidHandle())
			RightCharacter->SetOnPropertyValueChanged(OnValueChanged);
	
	Category.AddProperty(LevelProp);
	Category.AddProperty(DialogueProp);
	
	Category.AddGroup(FName("Line Info"), FText::AsNumber(1))
	.HeaderRow()
	.NameContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		SNew(STextBlock).Text(FText::FromString("Global Display Settings"))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(10.f, 0.f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center)
			[
				SNew(STextBlock).Text(FText::FromString("Left Character"))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
			]
			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Fill)
			[
				LeftCharacterWidget
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center)
			[
				SNew(STextBlock).Text(FText::FromString("Right Character"))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
				
			]
			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Fill)
			[
				RightCharacterWidget
			]				
		]
	];

	Category.AddCustomBuilder(LinesUI(DetailBuilder, OnValueChanged));
}

TSharedRef<FDetailArrayBuilder> FDialogueDetailsCustomization::LinesUI(IDetailLayoutBuilder& DetailBuilder, FSimpleDelegate& OnValueChanged)
{
	TSharedRef<IPropertyHandle> LinesProp = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UPlayDialog, Lines));

	DetailBuilder.HideProperty(LinesProp);

	TSharedRef<FDetailArrayBuilder> ArrayBuilder = MakeShareable(
		new FDetailArrayBuilder(LinesProp, true,false, false));
	
	ArrayBuilder->OnGenerateArrayElementWidget(
	FOnGenerateArrayElementWidget::CreateLambda(
	[this, OnValueChanged](TSharedRef<IPropertyHandle> ElementProp, int32 ArrayIndex, IDetailChildrenBuilder& ChildrenBuilder)
	{				
		UObject* Obj = nullptr;
		ElementProp->GetValue(Obj);
		UDialogueLine* Line = Cast<UDialogueLine>(Obj);
				
		TSharedPtr<IPropertyHandle> ExpressionHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, Expression));
		TSharedRef<SWidget> ExpressionWidget = ExpressionHandle.IsValid()? ExpressionHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		TSharedPtr<IPropertyHandle> SkipMethodHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, LineSkipMethod));
		TSharedRef<SWidget> SkipMethodWidget = SkipMethodHandle.IsValid()? SkipMethodHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		TSharedPtr<IPropertyHandle> SkipWaitDurationHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, WaitDuration));
		TSharedRef<SWidget> SkipWaitDurationWidget = SkipWaitDurationHandle.IsValid() ? SkipWaitDurationHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		SkipWaitDurationWidget = Line->LineSkipMethod == ELineSkipMethod::AUTO_SKIP? SkipWaitDurationWidget: SNew(SBox);

		if (ExpressionHandle.IsValid())
			ExpressionHandle->SetOnPropertyValueChanged(OnValueChanged);
		if (SkipMethodHandle.IsValid())
			SkipMethodHandle->SetOnPropertyValueChanged(OnValueChanged);		
		if (SkipWaitDurationHandle.IsValid())
			SkipWaitDurationHandle->SetOnPropertyValueChanged(OnValueChanged);
		
				
		TSharedPtr<FSlateBrush> PortraitImage = MakeShared<FSlateBrush>();
		TSharedPtr<FSlateBrush> OtherPortraitImage = MakeShared<FSlateBrush>();
		
		TSharedRef<SWidget> OtherPortrait = SNew(SImage).DesiredSizeOverride(FVector2D(0.f));
		if (Line)
		{
			TSoftObjectPtr<UTexture2D> ResolvedPortrait = Line->ResolvePortraitForCurrentExpression(true);
			if (!ResolvedPortrait.IsNull())
			{
				UTexture2D* LoadedTexture = ResolvedPortrait.LoadSynchronous();
				if (LoadedTexture)
				{
					PortraitImage->SetResourceObject(LoadedTexture);
					PortraitBrushes.Add(PortraitImage);
				}
			}			
			if (Line->OtherCharacterID != "None")
			{
				TSoftObjectPtr<UTexture2D> OtherResolvedPortrait = Line->ResolvePortraitForCurrentExpression(false);
				if (!OtherResolvedPortrait.IsNull())
				{
					UTexture2D* OtherLoadedTexture = OtherResolvedPortrait.LoadSynchronous();
					if (OtherLoadedTexture)
					{
						OtherPortraitImage->SetResourceObject(OtherLoadedTexture);
						OtherPortrait = SNew(SImage).Image(OtherPortraitImage.Get()).DesiredSizeOverride(FVector2D(120.0f, 170.0f));
						PortraitBrushes.Add(OtherPortraitImage);
					}
				}
			}
		}
		
		ChildrenBuilder.AddGroup(FName("Line Info"), FText::AsNumber(ArrayIndex))
		.HeaderRow()
		.NameContent()
		.HAlign(HAlign_Left)
		.MaxDesiredWidth(120)
		.MinDesiredWidth(120)
		[
			SNew(SImage)
			.Image(PortraitImage.Get())
			.DesiredSizeOverride(FVector2D(120.0f, 170.0f))				
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)
				
				+ SVerticalBox::Slot()
				.FillHeight(.3)
				.Padding(10.f, 10.f, 10.f, 5.f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1)
						.HAlign(HAlign_Fill)
						.Padding(10.f, 0.f)
						[
							ExpressionWidget				
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1)
						.HAlign(HAlign_Fill)
						.Padding(10.f, 0.f, 0.f, 0.f)
						[
							SkipMethodWidget				
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1)
						.HAlign(HAlign_Fill)
						.FillWidth(.3)
						.Padding(3.f, 0.f, 10.f, 0.f)
						[
							SkipWaitDurationWidget				
						]
					]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1)
				.Padding(10.f, 0.f, 10.f, 10.f)
				[
					SNew(SBorder)
					.HAlign(HAlign_Fill)
					.BorderBackgroundColor(FLinearColor::Black)
					.Content()
					[
						SNew(SScrollBox)
						.Orientation(Orient_Vertical)
						.AllowOverscroll(EAllowOverscroll::Yes)
						.AnimateWheelScrolling(true)
						.NavigationScrollPadding(5)
						+ SScrollBox::Slot()
						[
			                SNew(STextBlock)
			                .AutoWrapText(true)
			                .ColorAndOpacity(Line->DebugColor)
							.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12))
			                .Text_Lambda([Line]() -> FText
			                {
                				if (!Line) return FText::GetEmpty();
                				return FText::Format(
                					FText::FromString("{0}: {1}"),
                					Line->Name,
                					Line->Line);
			                })
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(.4f)
			[
				OtherPortrait
			]
		];

	}));
	return ArrayBuilder;
}
