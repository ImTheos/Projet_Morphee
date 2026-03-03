// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Runner/MonsterRunnerAttack.h"
#include "AI/Characters/Runner/RunnerCharacter.h"

void UMonsterRunnerAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(MeshComp->GetOwner());
	
	if (!IsValid(RunnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterRunnerAttack::NotifyBegin : RunnerCharacter is invalid"));
		return;
	}
	
	if (!IsValid(RunnerCharacter->attackHitbox))
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterRunnerAttack::NotifyBegin : RunnerCharacter hitbox is invalid"));
		return;
	}
	
	RunnerCharacter->attackHitbox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

void UMonsterRunnerAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(MeshComp->GetOwner());
	
	if (!IsValid(RunnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterRunnerAttack::NotifyEnd : RunnerCharacter is invalid"));
		return;
	}
	
	if (!IsValid(RunnerCharacter->attackHitbox))
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterRunnerAttack::NotifyEnd : RunnerCharacter hitbox is invalid"));
		return;
	}
	
	RunnerCharacter->attackHitbox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}
