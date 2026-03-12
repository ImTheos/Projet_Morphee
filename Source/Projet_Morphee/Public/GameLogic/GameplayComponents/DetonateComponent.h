// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetonateComponent.generated.h"


class ABall;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_MORPHEE_API UDetonateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetonateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AssignBall(ABall* ball);
	
	UFUNCTION(BlueprintCallable)
	void Detonate();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float detonationCooldown = 1.0f;
	
private:
	UPROPERTY()
	TObjectPtr<ABall> ownedBall;
	
	float remainingDetonationTime;
};
