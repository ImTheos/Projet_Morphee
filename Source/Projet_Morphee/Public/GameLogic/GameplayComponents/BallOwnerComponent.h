// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallOwnerComponent.generated.h"


class ABall;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_MORPHEE_API UBallOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallOwnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AssignBall(ABall* ball);

	UPROPERTY(BlueprintReadOnly)
	float currentMaxBallDistance;

	UFUNCTION(BlueprintCallable)
	void EnableBallDistanceLimit(float distanceLimit);

	UFUNCTION(BlueprintCallable)
	void DisableBallDistanceLimit();

	UPROPERTY(EditAnywhere)
	bool limitBallDistanceOnSpawn;
	
	UPROPERTY()
	bool isLimitingBallDistance;
	
private:
	UPROPERTY()
	TObjectPtr<ABall> ownedBall;
	
	bool IsBallTooFar(const FVector& ownerActorLocation) const;
};
