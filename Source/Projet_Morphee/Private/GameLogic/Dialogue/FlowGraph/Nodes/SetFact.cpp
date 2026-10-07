#include "GameLogic/Dialogue/FlowGraph/Nodes/SetFact.h"

USetFact::USetFact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("CUSTOM");
#endif
}

void USetFact::ExecuteInput(const FName& PinName)
{
	UGameManager* gameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(this));
	if (!gameManager)
		return;
	gameManager->SetGameFactValue(FactTag, NewValue);
	TriggerFirstOutput(true);
}
