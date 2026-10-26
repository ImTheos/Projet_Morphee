// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Runner/EnemySpecialAttackNotifyState.h"
#include "AI/Characters/EnemyCharacter.h"

void UEnemySpecialAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Error, TEXT("UAttackNotifyState::NotifyBegin : EnemyCharacter is invalid"));
		return;
	}
	Enemy->OnSpecialAttackBegin();
}

void UEnemySpecialAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Error, TEXT("UAttackNotifyState::NotifyEnd : EnemyCharacter is invalid"));
		return;
	}
	Enemy->OnSpecialAttackEnd();
}
