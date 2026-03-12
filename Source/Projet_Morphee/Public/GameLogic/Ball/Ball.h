// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffects/BallEffect.h"
#include "GameFramework/Actor.h"
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
	virtual void TickAttract(float DeltaTime);
	virtual void TickGrab(float DeltaTime);
	
public:
	// Amount of units the balls travels per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties")
	float speed;
	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* directionWidget;
	
	// -------  -------  ------- 
	// ------- BALL STATE -------
	// -------  -------  ------- 
private:
	EBallState ballState = Free;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties")
	float minimumSpeed = 600.f;
	
	UPROPERTY()
	UObject* influenceSource;
	
	UFUNCTION(BlueprintCallable)
	EBallState GetBallState() const;
	
	// TODO : add option to set speed after state change to allow to remove ReleaseFromStationary too
	void SetBallState(EBallState newBallState, UObject* newInfluenceSource = nullptr);
	
	void ReleaseFromStationary(float releaseSpeed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties| Grab")
	float grabAnimDistance;
	
	// -------  -------  ------- 
	// ------- COLLISION ------- 
	// -------  -------  ------- 
private:
	void SetCollisionEnabled(bool enabled) const;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnCollisionBlock(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherHitComponent,
	                      FVector normalImpulse, const FHitResult& hit);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	FName hollowBallCollisionProfile = TEXT("HollowBall");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	FName regularBallCollisionProfile = TEXT("BouncingBall");;
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ball Effect")
	TSubclassOf<ABallEffect> defaultBallEffect;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Ball Effect")
	TSubclassOf<ABallEffect> ballEffect;
	
	UPROPERTY()
	ABallEffect* ballEffectInstance;
	
public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ABallEffect> GetBallEffect() const { return ballEffect; }
	
	UFUNCTION(BlueprintCallable)
	ABallEffect* GetBallEffectInstance() const { return ballEffectInstance; }
	
	/**
	 * Sets a new value for the ballEffect parameter
	 *
	 * @param newBallEffectClass  The new value ballEffect should have
	 * @param actualize Whether the ballEffect's "EffectApplied" function should be applied if the new effect is the same as the current version
	 */
	UFUNCTION(BlueprintCallable)
	void SetBallEffect(TSubclassOf<ABallEffect> newBallEffectClass, bool actualize = false);
	
	// -------  -------  ------- 
	// ------- BALL IS HIT -------
	// -------  -------  ------- 
public:
	UFUNCTION(BlueprintCallable)
	void BallHitByAttack(AActor* attacker);

	// -------  -------  ------- 
	// ------- ANIMATION -------
	// -------  -------  ------- 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties| Grab")
	float grabAnimationCycleDuration = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties| Grab")
	float grabAnimationHeight = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Properties| Grab")
	float grabAnimationSizeRatio = 0.8;
	
private:
	float grabbedAnimationTimer = 0.f;
	
public:
	// // -------  -------  ------- 
	// ------- DIRECTION WIDGET -------
	// -------  -------  ------- 
	void UpdateDirectionWidgetHeight();
	
	double directionWidgetHeight;
	
	// -------  -------  ------- 
	// ------- BALL MESH -------
	// -------  -------  -------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* ballMeshReference;
};
