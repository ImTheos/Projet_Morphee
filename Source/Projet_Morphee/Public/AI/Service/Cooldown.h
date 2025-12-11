// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Cooldown.generated.h"

/**
 * Service node that counts down a float value to zero
 */
UCLASS()
class PROJET_MORPHEE_API UCooldown : public UBTService
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector cooldownKey;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
