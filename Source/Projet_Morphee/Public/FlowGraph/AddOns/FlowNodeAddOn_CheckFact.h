#pragma once
#include "AddOns/FlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "Global/GameManager.h"
#include "FlowNodeAddOn_CheckFact.generated.h"

// Forward Declarations
class UFlowNode;

UCLASS(NotBlueprintable, meta = (DisplayName = "CHECKFACT"))
class PROJET_MORPHEE_API UFlowNodeAddOn_CheckFact
	: public UFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:
	virtual void ExecuteInput(const FName& PinName) override;
	UPROPERTY(EditAnywhere, meta = (Categories = "Fact"))
	FGameplayTag FactTag;
	
	UFlowNodeAddOn_CheckFact();

	// UFlowNodeBase
	virtual EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate, const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const override;
	// --

	// IFlowPredicateInterface
	virtual bool EvaluatePredicate_Implementation() const override;
};
