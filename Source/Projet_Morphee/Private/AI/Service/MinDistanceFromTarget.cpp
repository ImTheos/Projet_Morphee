// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Service/MinDistanceFromTarget.h"

#include <string>

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


void UMinDistanceFromTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	
	FVector targetLocation;
	
	TSubclassOf<UBlackboardKeyType> targetKeyType = blackboard->GetKeyType(targetKeyName.GetSelectedKeyID());
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
	
	float pathLength = CalculateDistance(aiCharacterActor, targetLocation);
	
	if (displayDistance)
	{
		const FString distanceDisplayMessage = FString::Printf(TEXT("%s : distance joueur =  L %.1f unités"), *aiCharacterActor->GetActorLabel(), pathLength);
		GEngine->AddOnScreenDebugMessage((uint64) aiCharacterActor->GetUniqueID(), 1.f, FColor::Yellow, distanceDisplayMessage);
	}
	
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		return;
	}
	
	if (displayRange)
	{
		DrawDebugCircle(world, aiCharacterActor->GetActorLocation(), distance, 200, 
						FColor::Yellow, false, 0.0f, 0, 0, 
						FVector(1, 0, 0), FVector(0, 1, 0));
	}

	const bool result = pathLength < distance;
	
	blackboard->SetValueAsBool(resultKeyName.SelectedKeyName, result);
}

void UMinDistanceFromTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	resultKeyName.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMinDistanceFromTarget, resultKeyName));
	targetKeyName.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMinDistanceFromTarget, targetKeyName));
	targetKeyName.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMinDistanceFromTarget, targetKeyName), AActor::StaticClass());
	
	const UBlackboardData* blackboardData = GetBlackboardAsset();
	
	if (!ensure(blackboardData))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::InitializeFromAsset : blackboardData is invalid"))
		return;
	}
	
	targetKeyName.ResolveSelectedKey(*blackboardData);
	resultKeyName.ResolveSelectedKey(*blackboardData);
}

float UMinDistanceFromTarget::CalculateDistance(AActor* aiCharacter, FVector targetLocation)
{
	float pathLength = 0.0f;

	switch (distanceType)
	{
	case NavDistance :
		{
			UWorld* world = GetWorld();
			if (!IsValid(world))
			{
				UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : world is invalid"));
				return pathLength;
			}
	
			UNavigationSystemV1* navigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
			if (!IsValid(navigationSystem))
			{
				UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navSystem is invalid"));
				return 0.0f;
			}
	
			const ANavigationData* navigationData = navigationSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
			if (!IsValid(navigationData))
			{
				UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navData is invalid"));
				return pathLength;
			}
	
			UNavigationPath* navigationPath =
				navigationSystem->FindPathToLocationSynchronously(	world, 
																	aiCharacter->GetActorLocation(), 
																	targetLocation, 
																	aiCharacter);
	
			if (!IsValid(navigationPath) || navigationPath->PathPoints.Num() < 2)
			{
				UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : invalid navigationPath"))
				return pathLength;
			}
	
			for (int i = 1; i < navigationPath->PathPoints.Num(); i++)
			{
				pathLength += FVector::Dist(navigationPath->PathPoints[i-1], navigationPath->PathPoints[i]);
			}
		}
		break;
		
	case RegularDistance:
		{
			pathLength =  FVector::Dist(aiCharacter->GetActorLocation(), targetLocation);
		}
		break;
	}
	
	return pathLength;
	
}
