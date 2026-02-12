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

void AEnemyCharacter::StunCharacter(float stunTime, bool autoUnStun)
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
	
	blackboard->SetValueAsFloat(bCanActKey, false);
	
	if (!autoUnStun)
	{
		return;
	}
	
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		return;
	}
	
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(this, &AEnemyCharacter::UnStunCharacter);
	
	world->GetTimerManager().SetTimer(timerHandle, timerDelegate, stunTime, false);
}

void AEnemyCharacter::UnStunCharacter()
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
	
	blackboard->SetValueAsFloat(bCanActKey, true);
}

