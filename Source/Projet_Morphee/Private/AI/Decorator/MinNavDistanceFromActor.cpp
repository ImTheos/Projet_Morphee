// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MinNavDistanceFromActor.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "AI/Navigation/NavigationDataInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"

bool UMinNavDistanceFromActor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	AAIController* controller = OwnerComp.GetAIOwner();
	if (!IsValid(controller))
	{
		return false;
	}
	
	APawn* currentActor = controller->GetPawn();
	if (!IsValid(currentActor))
	{
		return false;
	}
	
	// get targetactor
	AActor* targetActor;
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (!blackboard)
	{
		return false;
	}
	
	targetActor = Cast<AActor>(blackboard->GetValueAsObject(targetActorKeyName.SelectedKeyName));
	if (!IsValid(targetActor))
	{
		return false;
	}
	
	// TODO do this properly
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!IsValid(navSystem))
	{
		return false;
	}
	
	// TODO add world check
	ANavigationData* navData = navSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	
	float pathLength;
	
	navData->CalcPathLength(currentActor->GetActorLocation(), targetActor->GetActorLocation(), pathLength);
	
	return pathLength < distance;
}
