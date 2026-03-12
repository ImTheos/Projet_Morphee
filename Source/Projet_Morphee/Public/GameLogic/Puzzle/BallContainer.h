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
	ABallContainer();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="If true, the player will be able to take the ball back from the container by using the magnet"))
	bool canReleaseFromMagnet = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="If true, the ball will not be given back to the player if the distance with him exceeds the distance from the player's BallOwner component"))
	bool ignoreBallDistanceLimit = true;

	void BallReleased(ABall* releasedBall);
	
	// Output signal for blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BallReceivedSignal();
	
	// Output signal for blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BallReleasedSignal();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseBalls(float releaseSpeed);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent, int OtherBodyIndex);
	
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
	
	UPROPERTY()
	TArray<ABall*> releasedBalls;
};
