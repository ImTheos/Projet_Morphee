#pragma once

#include "Nodes/Actor/FlowNode_ComponentObserver.h"
#include "OnInteract_Observer.generated.h"

class UInteractionComponent;
class UFlowComponent;


UCLASS(NotBlueprintable, meta = (DisplayName = "On Interact"))
class UOnInteract_Observer : public UFlowNode_ComponentObserver
{
	GENERATED_UCLASS_BODY()
	
protected:
	TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>> ObservedInteractions;

protected:
	virtual void ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	virtual void ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	
protected:
	virtual void Cleanup() override;
};
