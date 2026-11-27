// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowComponent.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOverlapBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOverlapEnd);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJET_MORPHEE_API UInteractionComponent : public UFlowComponent
{
	GENERATED_BODY()
	public:
		UInteractionComponent();
 
		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction")
		FInteract OnInteractDelegate;
	
		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction")
		FOverlapBegin OnOverlapBeginDelegate;
		
		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction")
		FOverlapEnd OnOverlapEndDelegate;
};
