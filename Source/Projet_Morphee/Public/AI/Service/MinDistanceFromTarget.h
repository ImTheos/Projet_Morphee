// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MinDistanceFromTarget.generated.h"

UENUM(Blueprintable)
enum EDistanceType
{
	NavDistance,
	RegularDistance
};


UCLASS()
class PROJET_MORPHEE_API UMinDistanceFromTarget : public UBTService
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
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	EDistanceType distanceType;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool displayDistance = false;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool displayPath = false;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool displayRange = false;

private:
	float CalculateDistance(AActor* aiCharacter, FVector targetLocation);
	
};
