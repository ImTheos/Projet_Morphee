// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MinNavDistanceFromTarget.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UMinNavDistanceFromTarget::UMinNavDistanceFromTarget(const FObjectInitializer& OI) : Super(OI)
{
	targetKeyName.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, targetKeyName), AActor::StaticClass());
	targetKeyName.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, targetKeyName));
	
	resultKeyName.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMinNavDistanceFromTarget, resultKeyName));
}

void UMinNavDistanceFromTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (!blackboard)
	{
		return;
	}
	
	AAIController* controller = OwnerComp.GetAIOwner();
	if (!IsValid(controller))
	{
		return;
	}
	
	APawn* currentActor = controller->GetPawn();
	if (!IsValid(currentActor))
	{
		return;
	}
	
	FVector targetLocation;
	
	if (targetKeyName.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* targetActor = Cast<AActor>(blackboard->GetValueAsObject(targetKeyName.SelectedKeyName));
		if (!IsValid(targetActor))
		{
			return;
		}
		
		targetLocation = targetActor->GetActorLocation();
	}
	else if (targetKeyName.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		targetLocation = blackboard->GetValueAsVector(targetKeyName.SelectedKeyName);
	}
	else
	{
		return;
	}
	
	// TODO do this properly
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!IsValid(navSystem))
	{
		return;
	}
	
	// TODO add world check
	ANavigationData* navData = navSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	
	FVector::FReal pathLength;
	
	navData->CalcPathLength(currentActor->GetActorLocation(), targetLocation, pathLength);
	
	bool result = pathLength < distance;
	
	blackboard->SetValueAsBool(resultKeyName.SelectedKeyName, result);
}
