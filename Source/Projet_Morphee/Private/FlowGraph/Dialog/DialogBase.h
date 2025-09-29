// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MyCPPCharacter.h"
#include "Nodes/FlowNode.h"
#include "DialogBase.generated.h"

/**
 * 
 */
UCLASS()
class UDialogBase : public UFlowNode
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AMyCPPCharacter* playerCharacter;
	
	void InitPlayerCharacter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	FGameplayTag playerTag;
};
