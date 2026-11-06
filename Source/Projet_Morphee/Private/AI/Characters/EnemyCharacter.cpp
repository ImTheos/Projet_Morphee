// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/EnemyCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "GameLogic/Interfaces/Damageable.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	for (UActorComponent* Component : GetComponents())
	{
		if (Component && Component->IsA(USphereComponent::StaticClass()))
		{
			USphereComponent* hitbox = Cast<USphereComponent>(Component);
			if (!IsValid(Component))return;
			
			hitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
			hitbox->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnHitboxBlock);
			hitbox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
	}
}

void AEnemyCharacter::InitCharacter()
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
	
	blackboard->SetValueAsVector(baseLocationKey, GetActorLocation());
	
	UWorld* world = GetWorld();
	
	APlayerController* playerController = Cast<APlayerController>(world->GetFirstPlayerController());
	
	if (!IsValid(playerController))
	{
		return;
	}
	
	auto* playerCharacter = playerController->GetPawn();
	
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::InitCharacter : playerCharacter is invalid"));
		return;
	}
	
	blackboard->SetValueAsObject(targetActorKey, playerCharacter);
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

void AEnemyCharacter::OnHitboxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
	if (!IsValid(otherActor) || !IsValid(otherActor->GetClass()))
	{
		return;
	}
	
	if (!otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		return;
	}
	
	IDamageable::Execute_ReceiveDamage(	otherActor, 
										attackDamage, 
										sweepResult.ImpactPoint,
										sweepResult.ImpactNormal, this);
}

void AEnemyCharacter::OnHitboxBlock(UPrimitiveComponent* hitComponent, AActor* otherActor,
	UPrimitiveComponent* otherHitComponent, FVector normalImpulse, const FHitResult& hit)
{
	if (!IsValid(otherActor) || !IsValid(otherActor->GetClass()))
	{
		return;
	}
	
	if (!otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		return;
	}
	
	IDamageable::Execute_ReceiveDamage(	otherActor, 
										attackDamage, 
										hit.ImpactPoint,
										hit.ImpactNormal, this);
}
