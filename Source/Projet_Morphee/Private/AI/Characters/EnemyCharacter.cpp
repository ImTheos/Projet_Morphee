// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/EnemyCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCharacter::InitCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter::InitCharacter : this function should be overwritten and not called by Super()"))
}

void AEnemyCharacter::StunForSeconds(float stunDuration)
{
	AAIController* activeController = Cast<AAIController>(GetController());
	if (!IsValid(activeController))
	{
		return;
	}
	
	UBlackboardComponent* blackboard = activeController->GetBlackboardComponent();
	
	if (!IsValid(blackboard))
	{
		return;
	}
	
	blackboard->SetValueAsBool(bIsStunnedKey, true);
	
	UWorld* world = GetWorld();
	
	FTimerDelegate unstunDelegate;
	unstunDelegate.BindUObject(this, &AEnemyCharacter::Unstun);
	
	FTimerHandle unstunHandle;
	
	world->GetTimerManager().SetTimer(unstunHandle, unstunDelegate, stunDuration, false);
}

void AEnemyCharacter::Stun()
{
	AAIController* activeController = Cast<AAIController>(GetController());
	if (!IsValid(activeController))
	{
		return;
	}
	
	UBlackboardComponent* blackboard = activeController->GetBlackboardComponent();
	
	if (!IsValid(blackboard))
	{
		return;
	}
	
	blackboard->SetValueAsBool(bIsStunnedKey, true);
}

void AEnemyCharacter::Unstun()
{
	AAIController* activeController = Cast<AAIController>(GetController());
	if (!IsValid(activeController))
	{
		return;
	}
	
	UBlackboardComponent* blackboard = activeController->GetBlackboardComponent();
	
	if (!IsValid(blackboard))
	{
		return;
	}
	
	blackboard->SetValueAsBool(bIsStunnedKey, false);
}

