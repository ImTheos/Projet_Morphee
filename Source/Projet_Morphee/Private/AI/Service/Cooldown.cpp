// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Cooldown.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCooldown::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData* blackboardData = Asset.GetBlackboardAsset();
	
	cooldownKey.ResolveSelectedKey(*blackboardData);
}

void UCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!ensure(blackboard))
	{
		return;
	}
	
	float remainingCooldown = blackboard->GetValueAsFloat(cooldownKey.SelectedKeyName);
	
	remainingCooldown = FMath::Max(0.0f, remainingCooldown - DeltaSeconds);
	
	blackboard->SetValueAsFloat(cooldownKey.SelectedKeyName, remainingCooldown);
}
