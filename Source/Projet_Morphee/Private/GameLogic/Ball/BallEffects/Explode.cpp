// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffects/Explode.h"

#include "NiagaraFunctionLibrary.h"
#include "GameLogic/Ball/Ball.h"
#include "GameLogic/GameplayComponents/MagnetComponent.h"
#include "GameLogic/Interfaces/Damageable.h"

void UExplode::Detonate_Implementation(AActor* owner)
{
	Super::Detonate_Implementation(owner);
	
	ABall* ball = Cast<ABall>(owner);
	
	if (!IsValid(ball))
	{
		UE_LOG(LogTemp, Error, TEXT("Explode::Detonate : Invalid Ball"));
		return;
	}
	
	const UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		UE_LOG(LogTemp, Error, TEXT("UExplode::Detonate_Implementation : Invalid world"))
		return;
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(	world, explosionParticleSystem, 
													ball->GetActorLocation(), 
													FRotator::ZeroRotator,
													FVector(0.4));
	
	TArray<FHitResult> attackHitResults;
	const auto attackCollisionShape = FCollisionShape::MakeSphere(explosionRange);
	
	// TODO : Test for collision during the whole animation
	// This could be done by using a timer that calls a Sweep at every tick for the particle duration
	world->SweepMultiByChannel(attackHitResults, 
		ball->GetActorLocation(), ball->GetActorLocation(), 
		FQuat::Identity, explosionCollisionChannel, attackCollisionShape);
	
	for (FHitResult hitResult : attackHitResults)
	{
		auto* hitActor = Cast<AActor>(hitResult.GetActor());
		if (!hitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
		{
			continue;
		}
	
		IDamageable::Execute_ReceiveDamage(hitActor, explosionDamage, hitResult.ImpactNormal, ball);
	}

	const APlayerController* playerController = world->GetFirstPlayerController();
	
	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Error, TEXT("UExplode::Detonate_Implementation : Invalid player controller"));
		return;
	}

	const APawn* playerPawn = playerController->GetPawn();
	if (!IsValid(playerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("UExplode::Detonate_Implementation : Invalid player pawn"));
		return;
	}
	
	UMagnetComponent* magnetComponent = playerPawn->GetComponentByClass<UMagnetComponent>();
	
	if (!IsValid(magnetComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("UExplode::Detonate_Implementation : Invalid magnet component"));
		return;
	}
	
	magnetComponent->GrabAttractedObject();
	
	ball->SetActorLocation(playerPawn->GetActorLocation());
}
