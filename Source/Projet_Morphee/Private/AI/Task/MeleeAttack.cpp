// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameLogic/Interfaces/Damageable.h"

void UMeleeAttack::PreAttack()
{
	// Play animation startup
	// TODO : add real preview animation
	DrawDebugCircle(GetWorld(), aiCharacter->GetActorLocation(), blackboard->GetValueAsFloat("attackRange"), 
		100, FColor::Red, false, blackboard->GetValueAsFloat("attackStartupDuration"), 0, 10, 
		FVector(1,0,0), FVector(0,1,0));
	
	blackboard->SetValueAsFloat(remainingAttackCooldownKey.SelectedKeyName, blackboard->GetValueAsFloat("attackCooldown"));
	blackboard->SetValueAsBool(attackEndLagKey.SelectedKeyName, false);
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UMeleeAttack::Attack, blackboard->GetValueAsFloat("attackStartupDuration"));
}

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
	
	// Check if the player can attack right now
	if (remainingAttackCooldown != 0.0f)
	{
		return EBTNodeResult::Failed;
	}
	
	PreAttack();
	
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
	
	animInstance->Montage_Play(attackAnimationMontage);
	
	FOnMontageEnded animEndDelegate;
	animEndDelegate.BindUObject(this, &UMeleeAttack::EndAttackAnim);
	
	animInstance->Montage_SetEndDelegate(animEndDelegate, attackAnimationMontage);
	
	FHitResult attackHitResult;
	const auto attackCollisionShape = 
		FCollisionShape::MakeSphere(blackboard->GetValueAsFloat("attackRange"));
	
	// TODO : Allow this to be changed from editor
	constexpr ECollisionChannel attackTraceChannel = ECC_GameTraceChannel4;
	
	// TODO : Test for collision during the whole animation
	GetWorld()->SweepSingleByChannel(attackHitResult, 
		aiCharacter->GetActorLocation(), aiCharacter->GetActorLocation(), 
		FQuat::Identity, attackTraceChannel, attackCollisionShape);
	
	if (!attackHitResult.bBlockingHit)
	{
		// Attack did not hit a target
		return;
	}
	
	auto* hitActor = Cast<AActor>(attackHitResult.GetActor());
	if (!hitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		return;
	}
	
	IDamageable::Execute_ReceiveDamage(hitActor, 
		blackboard->GetValueAsFloat("attackDamage"), 
		attackHitResult.ImpactNormal, aiCharacter);
}

void UMeleeAttack::EndAttackAnim(UAnimMontage* animMontage, bool bInterrupted) const
{
	if (!blackboard)
	{
		return;
	}
	
	blackboard->SetValueAsBool(attackEndLagKey.SelectedKeyName, true);
}
