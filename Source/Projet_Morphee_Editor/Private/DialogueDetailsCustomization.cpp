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
	
	Category.AddProperty(LevelProp);
	Category.AddProperty(DialogueProp);
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
	[this, OnValueChanged, ArrayBuilder](TSharedRef<IPropertyHandle> ElementProp, int32 ArrayIndex, IDetailChildrenBuilder& ChildrenBuilder)
	{				
		UObject* Obj = nullptr;
		ElementProp->GetValue(Obj);
		UDialogueLine* Line = Cast<UDialogueLine>(Obj);

		TSharedPtr<IPropertyHandle> MainHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, Main));
		TSharedPtr<IPropertyHandle> OthersHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, Others));
		TSharedRef<FDetailArrayBuilder> OthersArrayBuilder = MakeShareable(new FDetailArrayBuilder(OthersHandle.ToSharedRef() , true,false, false));
		
		TSharedPtr<IPropertyHandle> ExpressionHandle;
		if (MainHandle.IsValid())
			ExpressionHandle = MainHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCharacterDialogInfo, Expression));
		
		TSharedRef<SWidget> ExpressionWidget = ExpressionHandle.IsValid() ? ExpressionHandle->CreatePropertyValueWidget() : SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		
		TSharedPtr<IPropertyHandle> SkipMethodHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, LineSkipMethod));
		TSharedRef<SWidget> SkipMethodWidget = SkipMethodHandle.IsValid()? SkipMethodHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		TSharedPtr<IPropertyHandle> SkipWaitDurationHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, WaitDuration));
		TSharedRef<SWidget> SkipWaitDurationWidget = SkipWaitDurationHandle.IsValid() ? SkipWaitDurationHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		SkipWaitDurationWidget = Line->LineSkipMethod == ELineSkipMethod::AUTO_SKIP? SkipWaitDurationWidget: SNew(SBox);
		TSharedPtr<IPropertyHandle> AnimateHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, bAnimateText));
		TSharedRef<SWidget> AnimateWidget = AnimateHandle.IsValid()? AnimateHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		TSharedPtr<IPropertyHandle> DelayHandle = ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, LetterDelay));
		TSharedRef<SWidget> DelayWidget = DelayHandle.IsValid()? DelayHandle->CreatePropertyValueWidget(): SNew(STextBlock).Text(FText::FromString("Invalid Expression"));
		DelayWidget = 	SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1).HAlign(HAlign_Right).VAlign(VAlign_Bottom)
						[
							SNew(STextBlock).Text(FText::FromString("Letter Delay")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(.8).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							DelayWidget
						];
		DelayWidget = Line->bAnimateText ? DelayWidget : SNew(STextBlock);
		
		if (SkipMethodHandle.IsValid())
			SkipMethodHandle->SetOnPropertyValueChanged(OnValueChanged);		
		if (SkipWaitDurationHandle.IsValid())
			SkipWaitDurationHandle->SetOnPropertyValueChanged(OnValueChanged);
		if (AnimateHandle.IsValid())
			AnimateHandle->SetOnPropertyValueChanged(OnValueChanged);
		if (DelayHandle.IsValid())
			DelayHandle->SetOnPropertyValueChanged(OnValueChanged);
		
		
		ChildrenBuilder.AddGroup(FName("Line Info"), FText::AsNumber(ArrayIndex))
		.HeaderRow()
		.NameContent()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			CharacterInfoUI(ElementProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueLine, Main)).ToSharedRef(), OnValueChanged, 1.3f, FLinearColor(0.018,0.019,0.022))
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().FillHeight(.4)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1)
						.HAlign(HAlign_Fill)
						.Padding(10.f, 0.f, 0.f, 0.f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Bottom)
							[
								SNew(STextBlock).Text(FText::FromString("Skip Method"))
								.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
							]
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Top)
							[
								SkipMethodWidget
							]	
						]
						+ SHorizontalBox::Slot().FillWidth(.4).HAlign(HAlign_Center)
						.Padding(3.f, 0.f, 0.f, 0.f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Bottom)
							[
								SNew(STextBlock).Text(FText::FromString(""))
								.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
							]
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Top)
							[
								SkipWaitDurationWidget
							]	
						]
						+ SHorizontalBox::Slot().FillWidth(.01)
						[
							SNew(SSeparator).Orientation(Orient_Vertical)
						]
						+ SHorizontalBox::Slot().FillWidth(1).HAlign(HAlign_Fill)
						.Padding(3.f, 0.f, 10.f, 0.f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center)
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().FillWidth(.1).HAlign(HAlign_Right).VAlign(VAlign_Bottom)
								[
									SNew(STextBlock).Text(FText::FromString("Animate Text"))
									.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
								]
								+ SHorizontalBox::Slot().FillWidth(.1).HAlign(HAlign_Left).VAlign(VAlign_Bottom)
								[
									AnimateWidget
								]
							]
							+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Fill).VAlign(VAlign_Top)
							[
								DelayWidget
							]	
						]
					]
					
				]
			]
			+ SVerticalBox::Slot().FillHeight(.7)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.BorderBackgroundColor(FLinearColor(0.015f, 0.015f, 0.015f))
				.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush"))
				.Padding(20)
				.Content()
				[
					SNew(SScrollBox)
					.Orientation(Orient_Vertical)
					.AllowOverscroll(EAllowOverscroll::Yes)
					.AnimateWheelScrolling(true)
					+ SScrollBox::Slot()
					[
		                SNew(STextBlock)
		                .AutoWrapText(true)
		                .ColorAndOpacity(Line->Main->DebugColor)
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12))
		                .Text_Lambda([Line]() -> FText
		                {
                			if (!Line) return FText::GetEmpty();
                			return FText::Format(
                				FText::FromString("{0} : {1}"),
                				Line->Main->Name,
                				Line->Line);
		                })
					]
				]
			]
		];
		ChildrenBuilder.AddGroup(FName("Line Info"), FText::FromString("Other Characters")).AddWidgetRow().ShouldAutoExpand(true)
		[
			OthersListUI(Line->Main->Name, OthersHandle, OnValueChanged)
			
		];
		ChildrenBuilder.AddGroup(FName("Line Info"), FText::FromString("")).HeaderRow()
		[
			SNew(SSeparator).Orientation(Orient_Horizontal).Thickness(0.01)
					
		];
		
	}));
	
	return ArrayBuilder;
}



TSharedRef<SScrollBox> FDialogueDetailsCustomization::OthersListUI(const FText name, const TSharedPtr<IPropertyHandle> OthersHandle,const FSimpleDelegate& OnValueChanged)
{
	TSharedRef<SScrollBox> OthersBox = SNew(SScrollBox).Orientation(Orient_Horizontal);

	if (OthersHandle.IsValid())
	{
	    TSharedPtr<IPropertyHandleArray> OthersArray = OthersHandle->AsArray();
	    if (OthersArray.IsValid())
	    {
	        OthersArray->SetOnNumElementsChanged(OnValueChanged);
			auto d = name.ToString();
	        uint32 Num = 0;
	        OthersArray->GetNumElements(Num);

	        for (uint32 i = 0; i < Num; ++i)
	        {
				TSharedRef<IPropertyHandle> OtherHandle = OthersArray->GetElement(i);
	        	
	            OthersBox->AddSlot().Padding(2.f).FillSize(1)
	            [
					CharacterInfoUI(OtherHandle, OnValueChanged, .8f, FLinearColor(0.019,0.018,0.022))
				];
	        }
	    }
	}
	return OthersBox;
}


TSharedRef<SBorder> FDialogueDetailsCustomization::CharacterInfoUI(TSharedRef<IPropertyHandle> Handle,const FSimpleDelegate& OnValueChanged,float PortraitSize = 1.0f, FLinearColor backgroundColor = FLinearColor::Black)
{
	UObject* Obj = nullptr;
	Handle->GetValue(Obj);
	UCharacterDialogInfo* Info = Cast<UCharacterDialogInfo>(Obj);
	        	
	TSharedPtr<FSlateBrush> PortraitImage = MakeShared<FSlateBrush>();
	TSharedRef<SWidget> Portrait = SNew(SImage).DesiredSizeOverride(FVector2D(0.f));
	if (Info)
	{
		TSoftObjectPtr<UTexture2D> ResolvedPortrait = Info->ResolvePortraitForCurrentExpression();
		if (!ResolvedPortrait.IsNull())
		{
			UTexture2D* LoadedTexture = ResolvedPortrait.LoadSynchronous();
			if (LoadedTexture)
			{
				PortraitImage->SetResourceObject(LoadedTexture);
				Portrait = SNew(SImage).Image(PortraitImage.Get()).DesiredSizeOverride(BaseCharacterImageRatio * PortraitSize);
				PortraitBrushes.Add(PortraitImage);
			}
		}			
	}

	TSharedPtr<IPropertyHandle> ExpressionHandle = Handle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCharacterDialogInfo, Expression));
	if (ExpressionHandle.IsValid())
		ExpressionHandle->SetOnPropertyValueChanged(OnValueChanged);
	        	
	TSharedPtr<IPropertyHandle> PositionHandle = Handle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCharacterDialogInfo, Position));
	if (PositionHandle.IsValid())
		PositionHandle->SetOnPropertyValueChanged(OnValueChanged);
	
	return SNew(SBorder).VAlign(VAlign_Center).HAlign(HAlign_Fill).BorderBackgroundColor(backgroundColor).BorderImage(FAppStyle::Get().GetBrush("WhiteBrush")).Content()
	[
		SNew(SHorizontalBox)
     	+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
     	[
     		Portrait
     	]
     	+ SHorizontalBox::Slot().FillWidth(1).HAlign(HAlign_Center)
     	[
     		SNew(SVerticalBox)
			 + SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Bottom)
			 [
     			SNew(SVerticalBox)
     			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Bottom)
     			[
     				SNew(STextBlock).Text(FText::FromString("Expression"))
     				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
     			]
     			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Top)
     			[
     				ExpressionHandle.IsValid() ? ExpressionHandle->CreatePropertyValueWidget() : SNullWidget::NullWidget
     			]				 
			 ]
			 + SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Top)
     		[
     			SNew(SVerticalBox)
     			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Bottom)
				[
					SNew(STextBlock).Text(FText::FromString("Position"))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 8))
				]
     			+ SVerticalBox::Slot().FillHeight(1).HAlign(HAlign_Center).VAlign(VAlign_Top)
     			[
     				PositionHandle.IsValid() ? PositionHandle->CreatePropertyValueWidget() : SNullWidget::NullWidget
     			]     			
     		]
     	]
	];
}
