#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "SaveGameSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Global/SaveSystem.h"
#include "GameManager.generated.h"

class UGlobalGameFacts;

UCLASS()
class UGameManager : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	bool GetGameFactValue(const FGameplayTag& gameFactID) const;
	void SetGameFactValue(bool value);
	void SaveGlobalData();
	void LoadGlobalData();
private:
	UGlobalGameFacts* gameFacts;
};


class UGlobalGameFacts
{
	public:
	
	UGlobalGameFacts();
	
	TMap<FString, bool> GameFactsDict;
	void ReconcileGameFactsAndGameplayTags();
	bool GetGameFactValue(FGameplayTag GameFactTag);
	void SetGameFactValue(FGameplayTag GameFactTag, bool value);
};