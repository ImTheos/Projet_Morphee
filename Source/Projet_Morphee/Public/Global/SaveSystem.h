#pragma once

#include "CoreMinimal.h"
#include "FlowSave.h"
#include "SaveSystem.generated.h"

UCLASS()
class PROJET_MORPHEE_API USaveSystem : public UFlowSaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, bool> GameFacts;
};
