// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/IsWithinRange.h"

void UIsWithinRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	targetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UIsWithinRange, targetKey), AActor::StaticClass());
	targetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UIsWithinRange, targetKey));
	
	const UBlackboardData* blackboardData = GetBlackboardAsset();
	
	if (!ensure(blackboardData))
	{
		UE_LOG(LogTemp, Error, TEXT("UIsWithinRange::InitializeFromAsset : blackboardData is invalid"))
		return;
	}
	
	targetKey.ResolveSelectedKey(*blackboardData);
	
}

bool UIsWithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	
}
