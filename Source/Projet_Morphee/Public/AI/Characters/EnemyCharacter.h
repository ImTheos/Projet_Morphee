// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// Base class for enemies, that gives root logic for AI behaviours
UCLASS()
class PROJET_MORPHEE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName bIsStunnedKey = TEXT("bIsStunned");
	
	UFUNCTION(BlueprintCallable)
	virtual void InitCharacter();
	
	UFUNCTION(BlueprintCallable)
	void Stun();
	UFUNCTION(BlueprintCallable)
	void Unstun();
	UFUNCTION(BlueprintCallable)
	void StunForSeconds(float stunDuration);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackBegin();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpecialAttackBegin();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpecialAttackEnd();
	
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
									int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnHitboxBlock(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
						  FVector normalImpulse, const FHitResult& hit);
	
	UPROPERTY(BlueprintReadOnly)
	bool IsAttackOngoing;
	
	/** Damage dealt by each attack **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(Category="Attack"))
	float attackDamage;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName baseLocationKey = TEXT("baseLocation");
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName targetActorKey = TEXT("targetActor");
};
