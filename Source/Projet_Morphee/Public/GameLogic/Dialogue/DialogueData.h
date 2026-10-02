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

//static std::unordered_map<FString,ECharacterExpression> const ExpressionFromStrings = { {"NEUTRAL",ECharacterExpression::NEUTRAL},{"FEAR",ECharacterExpression::FEAR},{"EXASPERATE",ECharacterExpression::EXASPERATE},{"FROWNING",ECharacterExpression::FROWNING},{"INTERROGATION",ECharacterExpression::INTERROGATION},{"MOCKING",ECharacterExpression::MOCKING},{"HURT",ECharacterExpression::HURT},{"SAD",ECharacterExpression::SAD} ,{"SMILE",ECharacterExpression::SMILE} ,{"SURPRISED",ECharacterExpression::SURPRISED} };

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


UCLASS(EditInlineNew, DefaultToInstanced, DontCollapseCategories)
class PROJET_MORPHEE_API UDialogueLine : public UObject
{
	GENERATED_BODY()
	UDialogueLine();
	
	public:
	static UDialogueLine* Create(UObject* Outer, FName TableId, FString& Key, FString& CharacterID, FString& OtherCharacterID);
	
	UPROPERTY(EditAnywhere, Category="Character Info",meta = (EditCondition = false))
	FString MainCharacterID;
	UPROPERTY(EditAnywhere, Category="Character Info", meta = (EditCondition = false))
	FText Name;
	UPROPERTY(EditAnywhere, Category="Image",meta = (GetOptions = "GetMainCharacterExpressionOptions"))
	FString Expression;
	UPROPERTY(EditAnywhere, Category="Text")
	ELineSkipMethod LineSkipMethod = ELineSkipMethod::WAIT_FOR_USER_SKIP;	
	UPROPERTY(EditAnywhere, Category="Text")
	float WaitDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category="Text")
	bool bAnimateText = true;
	UPROPERTY(EditAnywhere, Category="Text")
	float LetterDelay = 0.05;
	UPROPERTY(EditAnywhere, Category="Image", meta = (EditCondition = false))
	TSoftObjectPtr<UTexture2D> Portrait;
	UPROPERTY(EditAnywhere, Category="Text", meta = (MultiLine = true, EditCondition = false))
	FText Line;
	FLinearColor DebugColor;
	
	
	UPROPERTY(EditAnywhere, Category="Other",meta = (EditCondition = false))
	FString OtherCharacterID;
	UPROPERTY(EditAnywhere, Category="Other",meta = (GetOptions = "GetOtherCharacterExpressionOptions"))
	FString OtherCharacterExpression;
	UPROPERTY(EditAnywhere, Category="Other", meta = (EditCondition = false))
	TSoftObjectPtr<UTexture2D> OtherPortrait;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	TSoftObjectPtr<UTexture2D> ResolvePortraitForCurrentExpression(bool isMainCharacter);
	
private:
	FCharacterInfo* mainCharacterInfo;
	TMap<FString,ECharacterExpression> MainExpressionOptions;
	
	FCharacterInfo* otherCharacterInfo;
	TMap<FString,ECharacterExpression> otherExpressionOptions;
	
	
	UFUNCTION()
	TArray<FString> GetMainCharacterExpressionOptions();	
	UFUNCTION()
	TArray<FString> GetOtherCharacterExpressionOptions();	
	
	void UpdateCharacterData();
	bool TryUpdateCharacterInfo();
	void GetAvailableExpressionOptionsFromMain();
	void GetAvailableExpressionOptionsFromOther();
	
};

class PROJET_MORPHEE_API UDialogHelper
{
	static TWeakObjectPtr<UDataTable> CharactersData;
	
	public:
	static FCharacterInfo* TryGetCharacterInfo(const FString& CharacterID);
	static TArray<ECharacterExpression> GetAvailableExpressionOptionsFromCharacter(const FString& CharacterID);
	static TWeakObjectPtr<UDataTable> GetCharactersDatatable();
};
