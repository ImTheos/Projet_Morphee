// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagnetComponent.generated.h"

class AAttractable;
// Component that allows the owner to attract a specific entity
// from a distance and to grab it when the attracted object arrives
// in a certain range from the owner
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJET_MORPHEE_API UMagnetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMagnetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetAttractedObject(AAttractable* objectToAttract);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float magnetRadius;

private:
	UPROPERTY()
	AAttractable* attractedObject;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateMagnet();

	UFUNCTION(BlueprintCallable)
	void DeactivateMagnet();
	
	UFUNCTION(BlueprintCallable)
	void FreeBall();

private:
	// Pulls the tracked object towards its magnetic source and activates magneting
	void AttractObject();
	
	void GrabAttractedObject();
	
	bool isMagnetActive;
};
