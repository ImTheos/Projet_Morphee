// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/MeleeAttack.h"

#include "AIController.h"
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
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!blackboard)
	{
		return EBTNodeResult::Failed;
	} 
	
	float remainingCooldown = blackboard->GetValueAsFloat(remainingCooldownKey.SelectedKeyName);
	
	if (remainingCooldown != 0.0f)
	{
		return EBTNodeResult::InProgress;
	}
	
	blackboard->SetValueAsFloat(remainingCooldownKey.SelectedKeyName, cooldown);
	
	// play animation startup
	DrawDebugCircle(GetWorld(), pawn->GetActorLocation(), 100, 
		10000, FColor::Red, false, 1.0f, 0, 10, 
		FVector(1,0,0), FVector(0,1,0));
	
	// wait for startup to finish
	FTimerHandle timerHandle;
	
	FTimerDelegate attackDelegate;
	attackDelegate.BindUFunction(this, "Attack");
	
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
}

void UMeleeAttack::Attack() const
{
	USkeletalMeshComponent* meshComponent = pawn->GetComponentByClass<USkeletalMeshComponent>();
	
	if (!IsValid(meshComponent))
	{
		return;
	}
	
	// launch attack
	meshComponent->PlayAnimation(attackAnimation, false);
	
	// test for hit
	FHitResult hitResult;
	
	FCollisionShape collisionShape;
	collisionShape.MakeSphere(attackSphereSize);
	
	// TODO : un-hardcode the collision profile
	GetWorld()->SweepSingleByProfile(hitResult, pawn->GetActorLocation(), pawn->GetActorLocation(), FQuat::Identity, TEXT("Pawn"), collisionShape);
	
	if (hitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UMeleeAttack : Collision detected !"));
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("UMeleeAttack : No collision detected :("));
	}
	
	// TODO
	// send hit to player (if player hit) 
	
}
