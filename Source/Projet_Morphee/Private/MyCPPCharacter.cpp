// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "BasicAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

// Sets default values
AMyCPPCharacter::AMyCPPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 90.f);

//Capsule size

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -92.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent> (TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);

	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));



}

// Called when the game starts or when spawned
void AMyCPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent))
	{
	BasicAttributeSet = AbilitySystemComponent->GetSet<UBasicAttributeSet>();
	}

}

// Called every frame
void AMyCPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	safePositionRemainingCooldown -= DeltaTime;
	if (safePositionRemainingCooldown > 0.0f)
	{
		return;
	}

	

	// test if position if safe : if capsule of 1.5* player diameter and small amount down touches ground
	UCharacterMovementComponent* movementComponent = GetCharacterMovement();

	// test validité

	if (movementComponent->IsMovingOnGround())
	{
		lastSafeLocation = GetTransform().GetLocation();

		DrawDebugCapsule(GetWorld(), lastSafeLocation, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), GetCapsuleComponent()->GetUnscaledCapsuleRadius(), FQuat::Identity, FColor::Yellow);
	}

	// if yes, set to new safePosition
}

// Called to bind functionality to input
void AMyCPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

