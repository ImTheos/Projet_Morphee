// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Runner/RunnerCharacter.h"

#include "AIController.h"
#include "MyCPPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameLogic/Interfaces/Damageable.h"
#include "Kismet/GameplayStatics.h"

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	attackHitbox = FindComponentByClass<USphereComponent>();
	if (!IsValid(attackHitbox))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARunnerCharacter::BeginPlay : The attackHitbox is invalid"));
		return;
	}
	
	attackHitbox->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::OnHitboxOverlap);
	attackHitbox->OnComponentHit.AddDynamic(this, &ARunnerCharacter::OnHitboxBlock);
	attackHitbox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ARunnerCharacter::InitCharacter()
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
	
	blackboard->SetValueAsFloat(attackCooldownKey, attackCooldown);
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
		UE_LOG(LogTemp, Error, TEXT("ARunnerCharacter::InitCharacter : playerCharacter is invalid"));
		return;
	}
	
	blackboard->SetValueAsObject(targetActorKey, playerCharacter);
}

void ARunnerCharacter::OnHitboxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
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
										sweepResult.ImpactNormal, this);
}

void ARunnerCharacter::OnHitboxBlock(UPrimitiveComponent* hitComponent, AActor* otherActor,
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
										hit.ImpactNormal, this);
}
