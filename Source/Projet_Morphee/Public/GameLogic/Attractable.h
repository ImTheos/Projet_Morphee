// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractable.generated.h"

UCLASS()
class PROJET_MORPHEE_API AAttractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetNewAttractionSource(const AActor* newAttractionSource);

	bool IsBeingAttracted() const;

private:
	UPROPERTY()
	const AActor* attractionSource;

	bool isBeingAttracted;

};
