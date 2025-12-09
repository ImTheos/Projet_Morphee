// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/RunnerCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void ARunnerCharacter::InitCharacter()
{
	Super::InitCharacter();
	
	AAIController* activeController = Cast<AAIController>(GetController());
	if (!IsValid(activeController))
	{
		return;
	}
	
	UBlackboardComponent* blackboard = activeController->GetBlackboardComponent();
	
	blackboard->SetValueAsFloat(TEXT("attackRange"), attackRange);
	blackboard->SetValueAsFloat(TEXT("attackCooldown"), attackCooldown);
	blackboard->SetValueAsFloat(TEXT("attackStartupDuration"), attackStartupDuration);
	blackboard->SetValueAsFloat(TEXT("attackDamage"), attackDamage);
}
