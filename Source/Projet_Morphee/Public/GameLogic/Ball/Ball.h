// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffect.h"
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
	void OnCollision(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCollisionBP(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
	
	
	// -------  -------  ------- 
	// ------- BALL EFFECT -------
	// -------  -------  ------- 
public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UBallEffect> ballEffect;
	
	// TODO : move this function to an "explosive" ballEffect
	UFUNCTION(BlueprintCallable)
	void Explode(UNiagaraSystem* explosionParticleSystem, float explosionRange = 300.f, float explosionDamage = 100.f);
	
};
