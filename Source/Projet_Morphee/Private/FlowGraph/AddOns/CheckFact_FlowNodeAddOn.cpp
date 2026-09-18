
#include "Flowgraph/AddOns/CheckFact_FlowNodeAddOn.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CheckFact_FlowNodeAddOn)

void UCheckFact_FlowNodeAddOn::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
}

UCheckFact_FlowNodeAddOn::UCheckFact_FlowNodeAddOn()
	: Super(){}

EFlowAddOnAcceptResult UCheckFact_FlowNodeAddOn::AcceptFlowNodeAddOnChild_Implementation(
	const UFlowNodeAddOn* AddOnTemplate,
	const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const
{
	return EFlowAddOnAcceptResult::TentativeAccept;
}

bool UCheckFact_FlowNodeAddOn::EvaluatePredicate_Implementation() const
{
	UGameManager* gameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));
	if (!gameManager)
	{
		return false;
	}
	return(gameManager->GetGameFactValue(FactTag));
}
