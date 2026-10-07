#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Nodes/FlowNode.h"
#include "NativeGameplayTags.h"
#include "GameLogic/UI/DialogUI.h"
#include "Global/GameManager.h"
#include "SetFact.generated.h"


UCLASS()
class USetFact : public UFlowNode
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Fact"))
	FGameplayTag FactTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NewValue;
	
	virtual void ExecuteInput(const FName& PinName) override;
};