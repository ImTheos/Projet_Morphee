// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractable.generated.h"

UENUM()
enum EAttractableState
{
	Free,
	Attracted,
	Grabbed
};

UCLASS(Abstract)
class PROJET_MORPHEE_API AAttractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttractable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void TickAttract() {}
	virtual void TickGrab() {}

	void SetNewAttractionSource(const AActor* newAttractionSource);
	void SetNewGrabSource(const AActor* newGrabSource);;

	EAttractableState GetAttractionState() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float grabAnimDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;
	
protected:
	UPROPERTY()
	const AActor* attractionSource;

private:
	EAttractableState attractionState = Free;
	void SetCollisionEnabled(ECollisionEnabled::Type collisionType) const;

public:
	void FreeFromAttraction();
	
};
