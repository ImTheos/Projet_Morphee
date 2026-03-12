// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsWithinRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UIsWithinRange : public UBTDecorator
{
	GENERATED_BODY()
	
	FBlackboardKeySelector targetKey;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
