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
class PROJET_MORPHEE_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	virtual bool GetGameFactCompletedFromTag(const FGameplayTag& gameFactID);
	UFUNCTION(BlueprintCallable)
	virtual bool GetGameFactCompletedFromString(const FString& gameFactID);
	UFUNCTION(BlueprintCallable)
	virtual void SetGameFactCompletedFromTag(const FGameplayTag& gameFactID, bool value);
	UFUNCTION(BlueprintCallable)
	virtual void SetGameFactCompletedFromString(const FString& gameFactID, bool value);
	UFUNCTION(BlueprintCallable)
	virtual void SaveGlobalData();
	UFUNCTION(BlueprintCallable)
	virtual void LoadGlobalData();
	UPROPERTY(BlueprintReadOnly)
	UGlobalGameFacts* GameFacts;
	
};

UCLASS(Blueprintable, BlueprintType)
class UGlobalGameFacts : public UObject
{
	GENERATED_BODY()
	
	public:
	
	UGlobalGameFacts();
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, bool> GameFactsDict;
	UFUNCTION(BlueprintCallable)
	void ReconcileGameFactsAndGameplayTags();
	UFUNCTION(BlueprintCallable)
	bool GetGameFactCompleted(const FString& GameFactTag);
	UFUNCTION(BlueprintCallable)
	void SetGameFactCompleted(const FString& GameFactTag, bool value);
};