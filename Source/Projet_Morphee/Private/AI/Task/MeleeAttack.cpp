// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/MeleeAttack.h"

#include "AIController.h"
#include "MyCPPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	
	float remainingCooldown = blackboard->GetValueAsFloat(remainingCooldownKey.SelectedKeyName);
	
	if (remainingCooldown != 0.0f)
	{
		return EBTNodeResult::Failed;
	}
	
	blackboard->SetValueAsFloat(remainingCooldownKey.SelectedKeyName, cooldown);
	
	// play animation startup
	DrawDebugCircle(GetWorld(), pawn->GetActorLocation(), 100, 
		10000, FColor::Red, false, 1.0f, 0, 10, 
		FVector(1,0,0), FVector(0,1,0));
	
	// wait for startup to finish
	FTimerHandle timerHandle;
	
	blackboard->SetValueAsBool(canAttackKey.SelectedKeyName, false);
	
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UMeleeAttack::Attack, attackStartupDuration);
	
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
	
	remainingCooldownKey.ResolveSelectedKey(*BBAsset);
	canAttackKey.ResolveSelectedKey(*BBAsset);
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
	
	FCollisionShape collisionShape = FCollisionShape::MakeSphere(attackSphereSize);
	
	// TODO : un-hardcode the collision profile
	GetWorld()->SweepSingleByChannel(hitResult, pawn->GetActorLocation(), pawn->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel4, collisionShape);
	
	if (!hitResult.bBlockingHit)
	{
		return;
	}
	
	// TODO
	// send hit to player (if player hit)
	auto* playerCharacter = Cast<AMyCPPCharacter>(hitResult.GetActor());
	
}

void UMeleeAttack::EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const
{
	if (!blackboard)
	{
		return;
	}
	
	blackboard->SetValueAsBool(canAttackKey.SelectedKeyName, true);
}
