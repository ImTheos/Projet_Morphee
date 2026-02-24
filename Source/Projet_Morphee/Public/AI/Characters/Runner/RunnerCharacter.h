// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "RunnerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API ARunnerCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
public:
	/** Time needed between the beginning of an attack and the beginning of the next one **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackCooldown;
	
	/** Damage dealt by each attack **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackDamage;
	
	UPROPERTY()
	USphereComponent* attackHitbox;
	
	virtual void InitCharacter() override;
	
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
									int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	
	UFUNCTION()
	void OnHitboxBlock(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
						  FVector normalImpulse, const FHitResult& hit);
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName attackCooldownKey = TEXT("attackCooldown");
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName baseLocationKey = TEXT("baseLocation");
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName targetActorKey = TEXT("targetActor");
};
