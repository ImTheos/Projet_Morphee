// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameLogic/Interfaces/Damageable.h"

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aiController = OwnerComp.GetAIOwner();
	
	if (!aiController)
	{
		return EBTNodeResult::Failed;
	}
	
	pawn = aiController->GetPawn();
	
	if (!IsValid(pawn) || !IsValid(pawn->GetComponentByClass<USkeletalMeshComponent>()))
	{
		return EBTNodeResult::Failed;
	}
	
	blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!blackboard)
	{
		return EBTNodeResult::Failed;
	} 
	
	float remainingCooldown = blackboard->GetValueAsFloat(remainingAttackCooldownKey.SelectedKeyName);
	
	if (remainingCooldown != 0.0f)
	{
		return EBTNodeResult::Failed;
	}
	
	blackboard->SetValueAsFloat(remainingAttackCooldownKey.SelectedKeyName, blackboard->GetValueAsFloat("attackCooldown"));
	
	// play animation startup
	DrawDebugCircle(GetWorld(), pawn->GetActorLocation(), blackboard->GetValueAsFloat("attackRange"), 
		10000, FColor::Red, false, blackboard->GetValueAsFloat("attackStartupDuration"), 0, 10, 
		FVector(1,0,0), FVector(0,1,0));
	
	// wait for startup to finish
	FTimerHandle timerHandle;
	
	blackboard->SetValueAsBool(canMoveKey.SelectedKeyName, false);
	
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UMeleeAttack::Attack, blackboard->GetValueAsFloat("attackStartupDuration"));
	
	return EBTNodeResult::Succeeded;
}

void UMeleeAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData* BBAsset = GetBlackboardAsset();
	
	if (!ensure(BBAsset))
	{
		return;
	}
	
	remainingAttackCooldownKey.ResolveSelectedKey(*BBAsset);
	canMoveKey.ResolveSelectedKey(*BBAsset);
	
	remainingAttackCooldownKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UMeleeAttack, remainingAttackCooldownKey));
	canMoveKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMeleeAttack, canMoveKey));
}

void UMeleeAttack::Attack()
{
	USkeletalMeshComponent* meshComponent = pawn->GetComponentByClass<USkeletalMeshComponent>();
	
	if (!IsValid(meshComponent))
	{
		return;
	}
	
	UAnimInstance* AnimInstance = meshComponent->GetAnimInstance();
	
	if (!IsValid(AnimInstance))
	{
		return;
	}
	
	// launch attack
	AnimInstance->Montage_Play(attackAnimationMontage);
	
	FOnMontageEnded animEndDelegate;
	animEndDelegate.BindUObject(this, &UMeleeAttack::EndAttackAnim);
	
	AnimInstance->Montage_SetEndDelegate(animEndDelegate, attackAnimationMontage);
	
	// test for hit
	FHitResult hitResult;
	
	FCollisionShape collisionShape = FCollisionShape::MakeSphere(blackboard->GetValueAsFloat("attackRange"));
	
	// TODO : un-hardcode the collision profile
	GetWorld()->SweepSingleByChannel(hitResult, pawn->GetActorLocation(), pawn->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel4, collisionShape);
	
	if (!hitResult.bBlockingHit)
	{
		return;
	}
	
	// TODO
	// send hit to player (if player hit)
	auto* hitActor = Cast<AActor>(hitResult.GetActor());
	if (!hitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		return;
	}
	
	IDamageable::Execute_ReceiveDamage(hitActor, blackboard->GetValueAsFloat("attackDamage"), hitResult.ImpactNormal, pawn);
}

void UMeleeAttack::EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const
{
	if (!blackboard)
	{
		return;
	}
	
	blackboard->SetValueAsBool(canMoveKey.SelectedKeyName, true);
}
