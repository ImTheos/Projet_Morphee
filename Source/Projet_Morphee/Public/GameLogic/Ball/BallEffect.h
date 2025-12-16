// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BallEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJET_MORPHEE_API UBallEffect : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName effectName;
	
	bool operator==(const UBallEffect&) const;
};
