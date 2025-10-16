// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic/Attractable.h"
#include "Ball.generated.h"

UCLASS()
class PROJET_MORPHEE_API ABall : public AAttractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TickAttract() override;

	virtual void TickGrab() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ballSpeed;

	// TODO : find a better way to set this
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float epsilonDistance = 0.2f;
};
