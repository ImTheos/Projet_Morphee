// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallEffect.h"
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
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UBallEffect> ballEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TickAttract();

	virtual void TickGrab();

	void SetNewAttractionSource(const AActor* newAttractionSource);
	void SetNewGrabSource(const AActor* newGrabSource);
	void FreeFromAttraction();
	
	void SetStationaryAtLocation(const FVector& location);
	void ReleaseFromStationary(float releaseSpeed);
	
	UFUNCTION(BlueprintCallable)
	EBallState GetBallState() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float grabAnimDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;
	
	UFUNCTION(BlueprintCallable)
	void Explode();
	
private:
	UPROPERTY()
	const AActor* attractionSource;
	
	EBallState ballState = Free;
	void SetCollisionEnabled(ECollisionEnabled::Type collisionType) const;
	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* directionWidget;

public:

	// TODO : find a better way to set this
	float epsilonDistance = 20.0f;
};
