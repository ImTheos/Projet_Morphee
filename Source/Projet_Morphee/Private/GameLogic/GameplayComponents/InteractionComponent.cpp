// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/InteractionComponent.h"

#include "GameLogic/Interfaces/Interactable.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TriggerInteract()
{
	if (AActor* Owner = GetOwner())
	{
		if (Owner->Implements<UInteractable>())
		{
			IInteractable::Execute_OnInteract(Owner);
		}
	}
 
	OnInteract.Broadcast();
}



