// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagnetComponent.generated.h"

class ABall;
// Component that allows the owner to attract a specific entity
// from a distance and to grab it when the attracted object arrives
// in a certain range from the owner
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJET_MORPHEE_API UMagnetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMagnetComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AssignBall(ABall* ball);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float magnetRadius;

private:
	UPROPERTY()
	TObjectPtr<ABall> ownedBall;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateMagnet();

	UFUNCTION(BlueprintCallable)
	void DeactivateMagnet();
	
	UFUNCTION(BlueprintCallable)
	void FreeAttractedObject();
	
	void GrabAttractedObject();
	
	void InitMagnetCooldown();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float cooldown = 2.f;
	
private:
	// Pulls the tracked object towards its magnetic source and activates magneting
	void AttractObject();
	
	bool isMagnetActive;
	
	float remainingCooldown = 0.f;
};
