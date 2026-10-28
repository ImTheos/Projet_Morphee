#include "Global/GameManager.h"

void UGameManager::Init()
{
	Super::Init();
}

bool UGameManager::GetGameFactCompletedFromTag(const FGameplayTag& gameFactID)
{
	return GetGameFactCompletedFromString(gameFactID.ToString());
}

bool UGameManager::GetGameFactCompletedFromString(const FString& gameFactID)
{
	if (!GameFacts)
		return false;
	return GameFacts->GetGameFactCompleted(gameFactID);
}

void UGameManager::SetGameFactCompletedFromTag(const FGameplayTag& gameFactID, bool value) 
{
	SetGameFactCompletedFromString(gameFactID.ToString(), value);
}

void UGameManager::SetGameFactCompletedFromString(const FString& gameFactID, bool value)
{
	GameFacts->SetGameFactCompleted(gameFactID, value);
}

void UGameManager::SaveGlobalData()
{
	if (USaveSystem* SaveGameInstance = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass())))
	{
		SaveGameInstance->GameFacts = GameFacts->GameFactsDict;
		
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "GlobalData", 0))
		{
			UE_LOG(LogTemp, SetColor, TEXT("Global Game Data SAVED%s"), OutputDeviceColor::COLOR_GREEN);
		}
	}
}

void UGameManager::LoadGlobalData()
{
	GameFacts =	NewObject<UGlobalGameFacts>();
	if (USaveSystem* LoadedGame = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot("GlobalData", 0)))
	{
		UE_LOG(LogTemp, SetColor, TEXT("Global Game Data SAVED%s"), OutputDeviceColor::COLOR_GREEN);
		GameFacts->GameFactsDict = LoadedGame->GameFacts;
	}
}


// GameFacts


UGlobalGameFacts::UGlobalGameFacts()
{
}

void UGlobalGameFacts::ReconcileGameFactsAndGameplayTags()
{
	FGameplayTagContainer Container;
	UGameplayTagsManager::Get().RequestAllGameplayTags(Container, false);
	FGameplayTagContainer parent = FGameplayTagContainer(UGameplayTagsManager::Get().RequestGameplayTag(FName(TEXT("Flow.Fact")), false));
	Container = Container.Filter(parent);
	TArray<FGameplayTag> TagArray;
	Container.GetGameplayTagArray(TagArray);
	
	for (auto tag : TagArray)
	{
		GameFactsDict.FindOrAdd(tag.ToString());
	}
}

bool UGlobalGameFacts::GetGameFactCompleted(const FString& GameFactTag)
{
	if (!GameFactsDict.Contains(GameFactTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find game fact %s in database"), *GameFactTag);
		return false;
	}
	
	return *GameFactsDict.Find(GameFactTag);
}

void UGlobalGameFacts::SetGameFactCompleted(const FString& GameFactTag, bool value)
{
	if (GameFactsDict.Contains(GameFactTag))
		GameFactsDict[GameFactTag] = value;
	else
		GameFactsDict.Add(GameFactTag, value);
}
