
#include "Flowgraph/AddOns/FlowNodeAddOn_CheckFact.h"

#include "Global/GameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNodeAddOn_CheckFact)

UFlowNodeAddOn_CheckFact::UFlowNodeAddOn_CheckFact()
	: Super(){}

EFlowAddOnAcceptResult UFlowNodeAddOn_CheckFact::AcceptFlowNodeAddOnChild_Implementation(
	const UFlowNodeAddOn* AddOnTemplate,
	const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const
{
	return EFlowAddOnAcceptResult::TentativeAccept;
}

bool UFlowNodeAddOn_CheckFact::EvaluatePredicate_Implementation() const
{
	return UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGameManager>()->GetGameFactValue(FactTag);
}
