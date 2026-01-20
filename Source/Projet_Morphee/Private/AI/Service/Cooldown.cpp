// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Cooldown.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCooldown::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	cooldownKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UCooldown, cooldownKey));
	
	const UBlackboardData* blackboardData = GetBlackboardAsset();
	
	if (!ensure(blackboardData))
	{
		UE_LOG(LogTemp, Error, TEXT("UCooldown::InitializeFromAsset : blackboardData is invalid"))
		return;
	}
	
	cooldownKey.ResolveSelectedKey(*blackboardData);
}

void UCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!ensure(blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("UCooldown::TickNode : blackboard is invalid"))
		return;
	}
	
	float remainingCooldown = blackboard->GetValueAsFloat(cooldownKey.SelectedKeyName);
	
	remainingCooldown = FMath::Max(0.0f, remainingCooldown - DeltaSeconds);
	
	blackboard->SetValueAsFloat(cooldownKey.SelectedKeyName, remainingCooldown);
}
