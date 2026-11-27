#pragma once
#include "Nodes/FlowNode.h"
#include "PlayFreeDialog.generated.h"


class UPlayerUI;

UCLASS(DontCollapseCategories)
class PROJET_MORPHEE_API UPlayFreeDialog : public UFlowNode
{
	GENERATED_UCLASS_BODY()
	
	virtual void ExecuteInput(const FName& PinName) override;
private:
	UPROPERTY()
	UPlayerUI* PlayerUI;
public:
	UFUNCTION()
	void Complete();
	
	UPROPERTY(EditAnywhere)
	FText text;
	UPROPERTY(EditAnywhere)
	float duration = 2.0f;
};