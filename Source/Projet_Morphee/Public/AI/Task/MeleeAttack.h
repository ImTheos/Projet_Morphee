// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API UMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
public:
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* attackAnimationMontage;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	FBlackboardKeySelector remainingAttackCooldownKey;

	UPROPERTY(EditAnywhere, Category="Attack")
	FBlackboardKeySelector canMoveKey;
	
private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	UPROPERTY()
	APawn* pawn;
	
	UFUNCTION()
	void Attack();
	
	UFUNCTION()
	void EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const;
	
	UPROPERTY()
	UBlackboardComponent* blackboard;
};
