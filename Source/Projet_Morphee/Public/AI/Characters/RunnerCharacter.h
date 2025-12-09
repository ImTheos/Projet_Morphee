// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/EnemyCharacter.h"
#include "RunnerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_MORPHEE_API ARunnerCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	/** Radius of the attack sphere hitbox **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackRange;

	/** Time needed between the beginning of an attack and the beginning of the next one **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackCooldown;
	
	/** Duration of the attack windup, i.e. the time span where the attack indicator is displayed but no hitbox is active yet **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackStartupDuration;
	
	/** Damage dealt by each attack **/
	UPROPERTY(EditAnywhere, meta=(Category="Attack"))
	float attackDamage;
	
	virtual void InitCharacter() override;
};
