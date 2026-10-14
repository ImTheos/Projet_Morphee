#pragma once
#include <unordered_map>
#include "DialogueData.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DebugColor;
};

UENUM()
enum class ELineSkipMethod : uint8
{
	WAIT_FOR_USER_SKIP = 0,
	AUTO_SKIP = 1,
};

UENUM(BlueprintType)
enum class EPosition : uint8
{
	NONE = 0,
	FAR_LEFT = 1,
	LEFT = 2,
	RIGHT = 3,
	FAR_RIGHT = 4,
};

UCLASS(EditInlineNew, DefaultToInstanced, DontCollapseCategories)
class PROJET_MORPHEE_API UCharacterDialogInfo : public UObject
{
	GENERATED_BODY()
public:
	static UCharacterDialogInfo* Create(UObject* outer, const FString& id);
	
	UPROPERTY(EditAnywhere, Category="Character Info",meta = (EditCondition = false))
	FString ID;
	
	UPROPERTY(EditAnywhere, Category="Character Info", meta = (EditCondition = false))
	FText Name;
	
	UPROPERTY(EditAnywhere, Category="Character Info",meta = (GetOptions = "GetExpressionOptions"))
	FString Expression;
	
	UPROPERTY(EditAnywhere, Category="Character Info", meta = (EditCondition = false))
	TSoftObjectPtr<UTexture2D> Portrait;
	
	UPROPERTY(EditAnywhere, Category="Character Info")
	EPosition Position = EPosition::LEFT;

	FLinearColor DebugColor;	
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	TSoftObjectPtr<UTexture2D> ResolvePortraitForCurrentExpression();
	
private:
	FCharacterInfo* CharacterInfo;
	TMap<FString,ECharacterExpression> MainExpressionOptions;
	UFUNCTION()
	TArray<FString> GetExpressionOptions();	
	void UpdateCharacterData();
	bool TryUpdateCharacterInfo();
	void GetAvailableExpressionOptions();
};


UCLASS(EditInlineNew, DefaultToInstanced, DontCollapseCategories)
class PROJET_MORPHEE_API UDialogueLine : public UObject
{
	GENERATED_BODY()
	UDialogueLine();
	
	public:
	static UDialogueLine* Create(UObject* Outer, FName TableId, FString& Key, FString& CharacterID);
	
	UPROPERTY(EditAnywhere)
	UCharacterDialogInfo* Main;
	
	UPROPERTY(EditAnywhere)
	TArray<UCharacterDialogInfo*> Others;
	
	UPROPERTY(EditAnywhere, Category="Line Info")
	ELineSkipMethod LineSkipMethod = ELineSkipMethod::WAIT_FOR_USER_SKIP;	
	UPROPERTY(EditAnywhere, Category="Line Info")
	float WaitDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category="Line Info")
	bool bAnimateText = true;
	UPROPERTY(EditAnywhere, Category="Line Info")
	float LetterDelay = 0.05;
	UPROPERTY(EditAnywhere, Category="Line Info", meta = (MultiLine = true, EditCondition = false))
	FText Line;
};

class PROJET_MORPHEE_API UDialogHelper
{
	static TWeakObjectPtr<UDataTable> CharactersDatabase;
	
	public:
	static FCharacterInfo* TryGetCharacterInfo(const FString& CharacterID);
	static TArray<ECharacterExpression> GetAvailableExpressionOptionsFromCharacter(const FString& CharacterID);
	static TWeakObjectPtr<UDataTable> GetCharactersDatatable();
};
