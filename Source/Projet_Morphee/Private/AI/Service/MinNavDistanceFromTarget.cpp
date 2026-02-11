// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/MinNavDistanceFromTarget.h"

#include <string>

#include "AIController.h"
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

	const APawn* aiCharacterActor = aiController->GetPawn();
	if (!IsValid(aiCharacterActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : aiCharacterActor is invalid"));
		return;
	}
	
	FVector targetLocation;
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (!blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : blackboard is invalid"));
		return;
	}
	
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
	
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : world is invalid"));
		return;
	}
	
	// TODO refactor this distance calculation by using the EQS system
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	if (!IsValid(navSystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navSystem is invalid"));
		return;
	}
	
	// TODO add world check
	
	const ANavigationData* navData = navSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!IsValid(navData))
	{
		UE_LOG(LogTemp, Error, TEXT("UMinNavDistanceFromTarget::TickNode : navData is invalid"));
		return;
	}
	
	FVector::FReal pathLength;
	
	navData->CalcPathLength(aiCharacterActor->GetActorLocation(), targetLocation, pathLength);
	
	if (displayDistance)
	{
		FString debugMessage = FString::Printf(TEXT("PathLength: %f"), pathLength);
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, debugMessage);
	}

	const bool result = pathLength < distance;
	
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
