// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Nodes/FlowNode.h"
#include "NativeGameplayTags.h"
#include "GameLogic/UI/DialogUI.h"
#include "Global/GameManager.h"
#include "SetFactNode.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(DefaultPlayerTag);

/**
 * 
 */
UCLASS()
class USetFactNode : public UFlowNode
{
	GENERATED_BODY()

protected:	
	bool TryInitialize();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Fact"))
	FGameplayTag FactTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NewValue;
	
	virtual void ExecuteInput(const FName& PinName) override;
};