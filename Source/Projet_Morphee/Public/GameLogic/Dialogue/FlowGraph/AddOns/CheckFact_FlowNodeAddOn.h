#pragma once
#include "AddOns/FlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "Global/GameManager.h"
#include "CheckFact_FlowNodeAddOn.generated.h"

// Forward Declarations
class UFlowNode;

UCLASS(NotBlueprintable, meta = (DisplayName = "CHECKFACT"))
class PROJET_MORPHEE_API UCheckFact_FlowNodeAddOn
	: public UFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:
	virtual void ExecuteInput(const FName& PinName) override;
	UPROPERTY(EditAnywhere, meta = (Categories = "Flow.Fact"))
	FGameplayTag FactTag;
	
	UCheckFact_FlowNodeAddOn();

	// UFlowNodeBase
	virtual EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate, const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const override;
	// --

	// IFlowPredicateInterface
	virtual bool EvaluatePredicate_Implementation() const override;
};
