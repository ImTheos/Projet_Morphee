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
	
	TArray<FHitResult> attackHitResults;
	const auto attackCollisionShape = FCollisionShape::MakeSphere(explosionRange);
	
	// TODO : Allow this to be changed from editor
	constexpr ECollisionChannel attackTraceChannel = ECC_GameTraceChannel3;
	
	// TODO : Test for collision during the whole animation
	GetWorld()->SweepMultiByChannel(attackHitResults, 
		ball->GetActorLocation(), ball->GetActorLocation(), 
		FQuat::Identity, attackTraceChannel, attackCollisionShape);
	
	if (attackHitResults.IsEmpty())
	{
		// Attack did not hit a target
		return;
	}
	
	for (FHitResult hitResult : attackHitResults)
	{
		auto* hitActor = Cast<AActor>(hitResult.GetActor());
		if (!hitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
		{
			continue;
		}
	
		IDamageable::Execute_ReceiveDamage(hitActor, explosionDamage, hitResult.ImpactNormal, ball);
	}
	
	// Spawn Niagara System
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionParticleSystem, ball->GetActorLocation(), FRotator::ZeroRotator,
		FVector(0.4));
	
	// TODO : do this properly
	// Get player character
	APawn* playerCharacter = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(playerCharacter))
	{
		return;
	}
	
	UMagnetComponent* magnetComponent = playerCharacter->GetComponentByClass<UMagnetComponent>();
	
	if (!IsValid(magnetComponent))
	{
		return;
	}
	
	// Set attracted object
	magnetComponent->AssignBall(ball);
	
	// Grab attracted object
	magnetComponent->GrabAttractedObject();
	
	ball->SetActorLocation(playerCharacter->GetActorLocation());
}
