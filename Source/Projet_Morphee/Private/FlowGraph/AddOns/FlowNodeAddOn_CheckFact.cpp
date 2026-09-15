
#include "Flowgraph/AddOns/FlowNodeAddOn_CheckFact.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNodeAddOn_CheckFact)

void UFlowNodeAddOn_CheckFact::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
}

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
	UGameManager* gameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));
	if (!gameManager)
	{
		return false;
	}
	return(gameManager->GetGameFactValue(FactTag));
}
