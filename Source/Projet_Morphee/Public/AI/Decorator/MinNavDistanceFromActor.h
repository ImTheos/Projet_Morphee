// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MinNavDistanceFromActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UMinNavDistanceFromActor : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector targetActorKeyName;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	float distance;
	
};
