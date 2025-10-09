// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "MyCPPCharacter.generated.h"

class UPlayerUI;
class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJET_MORPHEE_API AMyCPPCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	double safePositionRemainingCooldown = 0.0f;

public:
	// Sets default values for this character's properties
	AMyCPPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UBasicAttributeSet* BasicAttributeSet;

	UPROPERTY(BlueprintReadOnly)
	FVector lastSafeLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator lastSafeRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadWrite)
	UPlayerUI* playerUIWidget;

	UPROPERTY(EditAnywhere)
	double safePositionCheckCooldown = 2.0f;
};
