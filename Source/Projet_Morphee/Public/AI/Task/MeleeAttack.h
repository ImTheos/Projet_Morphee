// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MeleeAttack.generated.h"

/**
 * When called, this character attacks with the attackAnimationMontage animation after attackStartupDuration seconds
 * The attacks hits in a attackRange radius sphere, and prevents this character fron attacking again for attackCooldown seconds
 * remainingAttackCooldownKey should be ticked down using the Cooldown service node
 * attackEndLagKey should be used to prevent this character from acting when valued to false (attack endlag)
 */
UCLASS()
class PROJET_MORPHEE_API UMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Setup of variables and blackboard values
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
public:
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* attackAnimationMontage;
	
	/** Blackboard key that stores the remaining cooldown before the player can attack again **/
	UPROPERTY(EditAnywhere, Category="Attack")
	FBlackboardKeySelector remainingAttackCooldownKey;

	/** Blackboard key that stores the remaining cooldown before the player can act again **/
	UPROPERTY(EditAnywhere, Category="Attack")
	FBlackboardKeySelector attackEndLagKey;
	
private:
	UPROPERTY()
	ACharacter* aiCharacter;
	
	UPROPERTY()
	UBlackboardComponent* blackboard;
	
	// Attack hitbox preview
	void PreAttack();
	
	// Actual attack animation and hit detection 
	UFUNCTION()
	void Attack();
	
	// Allows the character to act again
	UFUNCTION()
	void EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const;
};
