// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MinNavDistanceFromTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UMinNavDistanceFromTarget : public UBTService
{
	GENERATED_BODY()
	
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector resultKeyName;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector targetKeyName;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	float distance;
	
};
