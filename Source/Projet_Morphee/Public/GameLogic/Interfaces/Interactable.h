// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class PROJET_MORPHEE_API IInteractable
{
	GENERATED_BODY()
public:

protected:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnInteract();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnBeginOverlap();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnEndOverlap();
};
