// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffects/BallEffect.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Ball.generated.h"

class UWidgetComponent;

UENUM(BlueprintType)
enum EBallState
{
	Free,
	Attracted,
	Grabbed,
	Stationary
};

UCLASS()
class PROJET_MORPHEE_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABall();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
private:
	virtual void TickAttract();
	virtual void TickGrab();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;
	
private:
	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* directionWidget;
	
	// -------  -------  ------- 
	// ------- BALL STATE -------
	// -------  -------  ------- 
private:
	EBallState ballState = Free;
	
	UPROPERTY()
	const UObject* influenceSource;
	
public:
	UFUNCTION(BlueprintCallable)
	EBallState GetBallState() const;
	
	// TODO : add option to set speed after state change to allow to remove ReleaseFromStationary too
	void SetBallState(EBallState newBallState, const UObject* newInfluenceSource = nullptr);
	
	void ReleaseFromStationary(float releaseSpeed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties")
	float grabAnimDistance;
	
	// -------  -------  ------- 
	// ------- COLLISION ------- 
	// -------  -------  ------- 
private:
	void SetCollisionEnabled(ECollisionEnabled::Type collisionType) const;


	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnCollisionBlock(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
	                      FVector normalImpulse, const FHitResult& hit);

public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Ball Collision Begin Overlap"))
	void OnCollisionBeginOverlapBP(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Ball Collision Hit"))
	void OnCollisionBlockBP(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
		FVector normalImpulse, FHitResult hit);
	
	// -------  -------  ------- 
	// ------- BALL EFFECT -------
	// -------  -------  ------- 
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBallEffect> defaultBallEffect;
	
private:
	TSubclassOf<UBallEffect> ballEffect;
	
public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UBallEffect> GetBallEffect() const { return ballEffect; }
	
	/**
	 * Sets a new value for the ballEffect parameter
	 *
	 * @param newBallEffect  The new value ballEffect should have
	 * @param actualize Whether the ballEffect's "EffectApplied" function should be applied if the new effect is the same as the current version
	 */
	UFUNCTION(BlueprintCallable)
	void SetBallEffect(TSubclassOf<UBallEffect> newBallEffect, bool actualize = false);
	
	// -------  -------  ------- 
	// ------- BALL IS HIT -------
	// -------  -------  ------- 
public:
	UFUNCTION(BlueprintCallable)
	void BallHitByAttack(AActor* attacker);
	
};
