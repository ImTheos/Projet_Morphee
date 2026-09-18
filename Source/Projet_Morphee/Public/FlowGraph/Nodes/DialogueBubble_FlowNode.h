#pragma once
#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "Engine/DataTable.h"
#include "DialogueBubble_FlowNode.generated.h"

UENUM()
enum ECharacterExpression : uint8
{
	NEUTRAL = 0,
	ANGRY = 1,
	FEAR = 2,
	EXASPERATE = 3,
	FROWNING = 4,
	INTERROGATION = 5,
	MOCKING = 6,
	HURT = 7,
	SAD = 8,
	SMILE = 9,
	SURPRISED = 10,
	COUNT UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(ECharacterExpression, ECharacterExpression::COUNT)

USTRUCT(BlueprintType)
struct PROJET_MORPHEE_API FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()
	FCharacterInfo();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<ECharacterExpression>, TSoftObjectPtr<UTexture2D>> Portraits;
};

UCLASS()
class PROJET_MORPHEE_API UDialogueBubble_FlowNode : public UFlowNode
{
	GENERATED_BODY()
public:
	bool isCharacterValid;
	bool isExpressionValid;
	
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetInitialCharacterOptions"))
	FString Character;
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetInitialExpressionOptions"))
	FString Expression;
	UPROPERTY(EditAnywhere, meta = (EditCondition = false))
	FText Name;
	UPROPERTY(EditAnywhere, meta = (EditCondition = false))
	TSoftObjectPtr<UTexture2D> Portrait;
	
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:
	FCharacterInfo* currentCharacterInfo;
	TMap<FString,ECharacterExpression> ExpressionOptions;
	
	
	TWeakObjectPtr<UDataTable> CharactersInfo;
	TWeakObjectPtr<UDataTable> GetDataTable();

	UFUNCTION()
	TArray<FString> GetInitialCharacterOptions();

	UFUNCTION()
	TArray<FString> GetInitialExpressionOptions();
	
};


