#include "GameLogic/Dialogue/FlowGraph/Nodes/OnInteract_Observer.h"
#include "GameLogic/GameplayComponents/InteractionComponent.h"

#include "FlowComponent.h"
#include "GameFramework/Actor.h"


UOnInteract_Observer::UOnInteract_Observer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
}

void UOnInteract_Observer::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!Actor.IsValid()) return;
	if (!ObservedInteractions.Contains(Actor))
	{
		TArray<UInteractionComponent*> FoundInteractions;
		Actor->GetComponents<UInteractionComponent>(FoundInteractions);
		
		if (FoundInteractions.Num() > 0)
		{
			RegisteredActors.Emplace(Actor, Component);
			
			ObservedInteractions.Emplace(Actor, FoundInteractions[0]);
			FoundInteractions[0]->OnInteractDelegate.AddDynamic(this, &UOnInteract_Observer::OnEventReceived); //// HERE ------------
		}
	}
}

void UOnInteract_Observer::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (const TWeakObjectPtr<UInteractionComponent>* Found = ObservedInteractions.Find(Actor))
	{
		if (UInteractionComponent* InteractionComponent = Found->Get())
		{
			InteractionComponent->OnInteractDelegate.RemoveAll(this);
		}
		ObservedInteractions.Remove(Actor);
	}
}

void UOnInteract_Observer::Cleanup()
{
	for (const TPair<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>>& Interaction : ObservedInteractions)
	{
		if (UInteractionComponent* InteractionComponent = Interaction.Value.Get())
		{
			InteractionComponent->OnInteractDelegate.RemoveAll(this);
		}
	}
	ObservedInteractions.Empty();
	Super::Cleanup();
}
