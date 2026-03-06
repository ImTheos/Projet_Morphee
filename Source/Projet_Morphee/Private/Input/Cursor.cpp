// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Cursor.h"

#include "MyCPPCharacter.h"
#include "Components/CapsuleComponent.h"

FVector ACursor::GetCursorWorldPosition()
{
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		return FVector::Zero();
	}
	
	APlayerController* playerController = world->GetFirstPlayerController();
	
	FVector mouseLocation, mouseDirection;
	
	playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
	
	AMyCPPCharacter* playerCharacter = Cast<AMyCPPCharacter>(playerController->GetPawn());
	if (!IsValid(playerCharacter))
	{
		return FVector::Zero();
	}
	
	FString debugString = FString::Printf(TEXT("Magnitude : %f"), mouseDirection.Length());
	GEngine->AddOnScreenDebugMessage(111, 1.0f, FColor::Yellow, debugString);
	
	if (mouseDirection.IsNearlyZero())
	{
		return FVector::Zero();
	}

	const double playerHeight = playerCharacter->GetActorLocation().Z - playerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	const FVector gameCursorPosition = mouseLocation + mouseDirection * (playerHeight - mouseLocation.Z) / mouseDirection.Z;
	
	return gameCursorPosition;
}

