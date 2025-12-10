// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCharacter::InitCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter::InitCharacter : this function should be overwritten and not called by Super()"))
}

