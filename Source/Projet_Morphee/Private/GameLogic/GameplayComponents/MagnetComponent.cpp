// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GameplayComponents/MagnetComponent.h"

#include "GameLogic/Attractable.h"

// Sets default values for this component's properties
UMagnetComponent::UMagnetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMagnetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMagnetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(attractedObject))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No attractedObject has been set"));
		return;
	}

	// get owner
	const AActor* componentOwner = GetOwner();

	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("MagnetComponent : the component owner is not valid (this should not happen)"))
	}

	// test distance between owner and magneted object
	if (FVector::Dist(componentOwner->GetActorLocation(), attractedObject->GetActorLocation()) > magnetRadius)
	{
		return;
	}

	if (!isMagnetActive && attractedObject->GetAttractionState() == Attracted)
	{
		attractedObject->FreeFromAttraction();
	}
	
	// grab object
	if (isMagnetActive)
	{
		GrabAttractedObject();
	}
}

void UMagnetComponent::SetAttractedObject(AAttractable* objectToAttract)
{
	attractedObject = objectToAttract;
}

void UMagnetComponent::AttractObject()
{
	const AActor* componentOwner = GetOwner();

	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : the component owner is not valid (this should not happen)"))
	}
	
	attractedObject->SetNewAttractionSource(componentOwner);
}

void UMagnetComponent::GrabAttractedObject()
{
	const AActor* componentOwner = GetOwner();
	if (!IsValid(componentOwner))
	{
		// how ?
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : the component owner is not valid (this should not happen)"))
	}

	if (!IsValid(attractedObject))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No attractedObject has been set"));
		return;
	}

	attractedObject->SetNewGrabSource(componentOwner);
}

void UMagnetComponent::ActivateMagnet()
{
	if (!IsValid(attractedObject))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No attractedObject has been set"));
		return;
	}
	
	isMagnetActive = true;

	// If object is already being attracted, no need to set up attraction again
	if (attractedObject->GetAttractionState() != Free)
	{
		return;
	}

	AttractObject();
}

void UMagnetComponent::DeactivateMagnet()
{
	isMagnetActive = false;
}

void UMagnetComponent::FreeAttractedObject()
{
	if (!IsValid(attractedObject))
	{
		UE_LOG(LogTemp, Error, TEXT("UMagnetComponent : No attractedObject has been set"));
		return;
	}

	attractedObject->FreeFromAttraction();
}

