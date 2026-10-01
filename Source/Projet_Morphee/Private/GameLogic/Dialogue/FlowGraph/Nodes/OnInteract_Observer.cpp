#include "GameLogic/Dialogue/FlowGraph/Nodes/OnInteract_Observer.h"
#include "GameLogic/GameplayComponents/InteractionComponent.h"

#include "FlowComponent.h"


UOnInteract_Observer::UOnInteract_Observer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
}

void UOnInteract_Observer::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!ObservedInteractions.Contains(Actor))
	{
		TArray<UInteractionComponent*> FoundInteractions;
		Actor->GetComponents<UInteractionComponent>(FoundInteractions);
		
		if (FoundInteractions.Num() > 0)
		{
			RegisteredActors.Emplace(Actor, Component);
			
			ObservedInteractions.Emplace(Actor, FoundInteractions[0]);
			FoundInteractions[0]->OnInteract.AddDynamic(this, &UOnInteract_Observer::OnEventReceived); //// HERE ------------
		}
	}
}

void UOnInteract_Observer::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	ensureAlways(ObservedInteractions.Contains(Component->GetOwner()));
	const TWeakObjectPtr<UInteractionComponent> InteractionComponent = ObservedInteractions[Component->GetOwner()];
	
	InteractionComponent->OnInteract.RemoveAll(this);
}

void UOnInteract_Observer::Cleanup()
{
	Super::Cleanup();

	for (const TPair<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>>& Interaction : ObservedInteractions)
	{
		Interaction.Value->OnInteract.RemoveAll(this);
	}
	ObservedInteractions.Empty();
}
