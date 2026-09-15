#include "Global/GameManager.h"

void UGameManager::Init()
{
	Super::Init();
}

bool UGameManager::GetGameFactValue(const FGameplayTag& gameFactID)
{
	if (!GameFacts)
		return false;
	return GameFacts->GetGameFactValue(gameFactID);
}

void UGameManager::SetGameFactValue(FGameplayTag& gameFactID, bool value) 
{
	GameFacts->SetGameFactValue(gameFactID, value);
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
	FGameplayTagContainer parent = FGameplayTagContainer(UGameplayTagsManager::Get().RequestGameplayTag(FName(TEXT("Fact")), false));
	Container = Container.Filter(parent);
	TArray<FGameplayTag> TagArray;
	Container.GetGameplayTagArray(TagArray);
	
	for (auto tag : TagArray)
	{
		GameFactsDict.FindOrAdd(tag.ToString());
	}
}

bool UGlobalGameFacts::GetGameFactValue(FGameplayTag GameFactTag)
{
	if (!GameFactsDict.Contains(GameFactTag.ToString()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find game fact %s in database"), *GameFactTag.ToString());
		return false;
	}
	
	return *GameFactsDict.Find(GameFactTag.ToString());
}

void UGlobalGameFacts::SetGameFactValue(FGameplayTag GameFactTag, bool value)
{
	if (GameFactsDict.Contains(GameFactTag.ToString()))
		GameFactsDict[GameFactTag.ToString()] = value;
	else
		GameFactsDict.Add(GameFactTag.ToString(), value);
}

//
