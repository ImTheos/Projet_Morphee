// Fill out your copyright notice in the Description page of Project Settings.


#include "NavVisualizer.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"

// Sets default values
ANavVisualizer::ANavVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavVisualizer::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ANavVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (!IsValid(playerPawn))
	{
		return;
	}
	
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (!IsValid(navSystem))
	{
		return;
	}
	
	UNavigationPath* navPath = navSystem->FindPathToLocationSynchronously(GetWorld(), playerPawn->GetActorLocation(), GetActorLocation(), playerPawn);
	
	if (!navPath || !navPath->IsValid() || navPath->PathPoints.Num() < 2)
	{
		return;
	}
	
	float pathLength = 0.f;
	for (int i = 1; i < navPath->PathPoints.Num(); i++)
	{
		pathLength += FVector::Dist(navPath->PathPoints[i-1], navPath->PathPoints[i]);
	}
	
	const ANavigationData* navData = navSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	
	FVector::FReal manualPathLength;
	
	navData->CalcPathLength(playerPawn->GetActorLocation(), GetActorLocation(), manualPathLength);
	
	FVector::FReal thirdPathLength;
	navSystem->GetPathLength(playerPawn->GetActorLocation(), GetActorLocation(), thirdPathLength);
	
	if (GEngine)
	{
		FString msg = FString::Printf(TEXT("Distance joueur - cible, nouvelle version :  L %.1f unités"), pathLength);
		GEngine->AddOnScreenDebugMessage((uint64) this, 0.f, FColor::Green, msg);
		
		FString msgManual = FString::Printf(TEXT("Distance joueur - cible, ancienne version : L %.1f unités"), manualPathLength);
		GEngine->AddOnScreenDebugMessage((uint64) this + 1, 0.f, FColor::Red, msgManual);
		
		FString msg3 = FString::Printf(TEXT("Distance joueur - cible, version 3 : L %.1f unités"), thirdPathLength);
		GEngine->AddOnScreenDebugMessage((uint64) this + 2, 0.f, FColor::Blue, msg3);
	}
	
	for (int i = 1; i < navPath->PathPoints.Num(); i++)
	{
		DrawDebugLine(GetWorld(), 
			navPath->PathPoints[i-1],
			navPath->PathPoints[i],
			FColor::Yellow,
			false,
			0.5f);
	}
}

