// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aiController = OwnerComp.GetAIOwner();
	
	if (!aiController)
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::ExecuteTask : aiController is invalid"));
		return EBTNodeResult::Failed;
	}
	
	aiCharacter = Cast<ACharacter>(aiController->GetPawn());
	
	if (!IsValid(aiCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::ExecuteTask : aiCharacter is invalid"));
		return EBTNodeResult::Failed;
	}
	
	blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::ExecuteTask : blackboard is invalid"));
		return EBTNodeResult::Failed;
	} 
	
	float remainingAttackCooldown = blackboard->GetValueAsFloat(remainingAttackCooldownKey.SelectedKeyName);
	
	if (remainingAttackCooldown > 0.0f)
	{
		return EBTNodeResult::Failed;
	}
	
	Attack();
	
	blackboard->SetValueAsFloat(remainingAttackCooldownKey.SelectedKeyName, blackboard->GetValueAsFloat("attackCooldown"));
	
	return EBTNodeResult::Succeeded;
}

void UMeleeAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	remainingAttackCooldownKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UMeleeAttack, remainingAttackCooldownKey));
	attackEndLagKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMeleeAttack, attackEndLagKey));

	const UBlackboardData* blackboardData = GetBlackboardAsset();
	
	if (!ensure(blackboardData))
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::InitializeFromAsset : blackboardData is invalid"))
		return;
	}
	
	// /!\ Necessary for Blackboards key to work correctly
	remainingAttackCooldownKey.ResolveSelectedKey(*blackboardData);
	attackEndLagKey.ResolveSelectedKey(*blackboardData);
}

void UMeleeAttack::Attack()
{
	USkeletalMeshComponent* meshComponent = aiCharacter->GetComponentByClass<USkeletalMeshComponent>();
	
	if (!IsValid(meshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::Attack : meshComponent is invalid"));
		return;
	}
	
	UAnimInstance* animInstance = meshComponent->GetAnimInstance();
	
	if (!IsValid(animInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeAttack::Attack : animInstance is invalid"));
		return;
	}
	
	blackboard->SetValueAsBool(attackEndLagKey.SelectedKeyName, true);
	
	animInstance->Montage_Play(attackAnimationMontage);
	
	FOnMontageEnded animEndDelegate;
	animEndDelegate.BindUObject(this, &UMeleeAttack::EndAttackAnim);
	
	animInstance->Montage_SetEndDelegate(animEndDelegate, attackAnimationMontage);
}

void UMeleeAttack::EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const
{
	if (!blackboard)
	{
		return;
	}
	
	blackboard->SetValueAsBool(attackEndLagKey.SelectedKeyName, false);
}
