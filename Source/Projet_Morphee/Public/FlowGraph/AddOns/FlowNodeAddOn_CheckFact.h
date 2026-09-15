#pragma once
#include "AddOns/FlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Kismet/GameplayStatics.h"
#include "FlowNodeAddOn_CheckFact.generated.h"

// Forward Declarations
class UFlowNode;

UCLASS(MinimalApi, NotBlueprintable, meta = (DisplayName = "CHECKFACT"))
class UFlowNodeAddOn_CheckFact
	: public UFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()
	virtual void ExecuteInput(const FName& PinName) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="DST.Pawn"), Category = "PawnInfo")
	FGameplayTag FactTag;
	
	UFlowNodeAddOn_CheckFact();

	// UFlowNodeBase
	virtual EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate, const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const override;
	// --

	// IFlowPredicateInterface
	virtual bool EvaluatePredicate_Implementation() const override;
};
