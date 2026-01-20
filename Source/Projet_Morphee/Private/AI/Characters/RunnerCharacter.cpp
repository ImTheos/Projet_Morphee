// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/RunnerCharacter.h"

#include "AIController.h"
#include "MyCPPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ARunnerCharacter::InitCharacter()
{
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
	
	// This location is set to the spawn location of the actor but this could be changed
	blackboard->SetValueAsVector(TEXT("baseLocation"), GetActorLocation());
	
	// Hacky way of getting the player character
	auto* playerCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCPPCharacter::StaticClass());
	
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("ARunnerCharacter::InitCharacter : playerCharacter is invalid"));
		return;
	}
	
	blackboard->SetValueAsObject(TEXT("targetActor"), playerCharacter);
}
