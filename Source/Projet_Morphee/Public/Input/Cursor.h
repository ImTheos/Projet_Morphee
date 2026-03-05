// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cursor.generated.h"

UCLASS()
class PROJET_MORPHEE_API ACursor : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FVector GetCursorWorldPosition();

};
