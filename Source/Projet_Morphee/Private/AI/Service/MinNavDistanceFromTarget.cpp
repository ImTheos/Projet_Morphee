// Fill out your copyright notice in the Description page of Project Settings.
UE_DISABLE_OPTIMIZATION

#include "AI/Service/MinNavDistanceFromTarget.h"

#include <string>

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


void UMinNavDistanceFromTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* aiController = OwnerComp.GetAIOwner();
	if (!IsValid(aiController))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : aiController is invalid"));
		return;
	}

	APawn* aiCharacterActor = aiController->GetPawn();
	if (!IsValid(aiCharacterActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : aiCharacterActor is invalid"));
		return;
	}
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (!blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : blackboard is invalid"));
		return;
	}
	
	TSubclassOf<UBlackboardKeyType> targetKeyType = blackboard->GetKeyType(targetKeyName.GetSelectedKeyID());
	
	FVector targetLocation;
	
	if (targetKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		const AActor* targetActor = Cast<AActor>(blackboard->GetValueAsObject(targetKeyName.SelectedKeyName));
		if (!IsValid(targetActor))
		{
			UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : targetActor is invalid"));
			return;
		}
		
		targetLocation = targetActor->GetActorLocation();
	}
	else if (targetKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		targetLocation = blackboard->GetValueAsVector(targetKeyName.SelectedKeyName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : invalid target key type"));
		return;
	}
	
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : world is invalid"));
		return;
	}
	
	UNavigationSystemV1* navigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	if (!IsValid(navigationSystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navSystem is invalid"));
		return;
	}
	
	const ANavigationData* navigationData = navigationSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!IsValid(navigationData))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navData is invalid"));
		return;
	}
	
	UNavigationPath* navigationPath = navigationSystem->FindPathToLocationSynchronously(GetWorld(), 
																		aiCharacterActor->GetActorLocation(), 
																		targetLocation, 
																		aiCharacterActor);
	
	if (!IsValid(navigationPath) || navigationPath->PathPoints.Num() < 2)
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : invalid navigationPath"))
		return;
	}
	
	float pathLength = 0.f;
	for (int i = 1; i < navigationPath->PathPoints.Num(); i++)
	{
		pathLength += FVector::Dist(navigationPath->PathPoints[i-1], navigationPath->PathPoints[i]);
	}
	
	if (displayDistance)
	{
		const FString distanceDisplayMessage = FString::Printf(TEXT("%s : distance joueur =  L %.1f unités"), *aiCharacterActor->GetActorLabel(), pathLength);
		GEngine->AddOnScreenDebugMessage((uint64) aiCharacterActor->GetUniqueID(), 1.f, FColor::Yellow, distanceDisplayMessage);
	}
	
	if (displayRange)
	{
		DrawDebugCircle(world, FMatrix::Identity, distance, 200, FColor::Yellow, false, 0.5f);
	}

	const bool result = pathLength < distance;
	
	
	if (result && displayPath)
	{
		for (int i = 1; i < navigationPath->PathPoints.Num(); i++)
		{
			DrawDebugLine(world, 
				navigationPath->PathPoints[i-1],
				navigationPath->PathPoints[i],
				FColor::Yellow,
				false,
				0.5f);
		}
	}
	
	blackboard->SetValueAsBool(resultKeyName.SelectedKeyName, result);
}

void UMinNavDistanceFromTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	resultKeyName.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, resultKeyName));
	targetKeyName.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, targetKeyName));
	targetKeyName.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, targetKeyName), AActor::StaticClass());
	
	const UBlackboardData* blackboardData = GetBlackboardAsset();
	
	if (!ensure(blackboardData))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::InitializeFromAsset : blackboardData is invalid"))
		return;
	}
	
	targetKeyName.ResolveSelectedKey(*blackboardData);
	resultKeyName.ResolveSelectedKey(*blackboardData);
} 
