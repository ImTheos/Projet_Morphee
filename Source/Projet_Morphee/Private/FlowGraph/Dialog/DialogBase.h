// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Nodes/FlowNode.h"
#include "NativeGameplayTags.h"
#include "GameLogic/UI/DialogUI.h"
#include "DialogBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(DefaultPlayerTag);

/**
 * 
 */
UCLASS()
class UDialogBase : public UFlowNode
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UDialogUI* dialogUI;
	
	void InitDialogUI();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	FGameplayTag playerTag = DefaultPlayerTag;
};
