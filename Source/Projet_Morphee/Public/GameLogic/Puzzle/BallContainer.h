// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallContainer.generated.h"

class ABall;
class UBoxComponent;

UCLASS()
class PROJET_MORPHEE_API ABallContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallContainer();

	UFUNCTION(BlueprintImplementableEvent)
	void OutputSignal();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BallReleasedSignal();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseBalls(float releaseSpeed);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* collisionComponent;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMeshPreview;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* containerMesh;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* sceneRoot;
	
	UPROPERTY()
	TArray<ABall*> storedBalls;
};
