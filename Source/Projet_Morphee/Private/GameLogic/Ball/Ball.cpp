// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/Ball.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameLogic/Interfaces/Damageable.h"
#include "GameLogic/Puzzle/BallContainer.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(defaultBallEffect))
	{
		SetBallEffect(defaultBallEffect);
	}
	
	USphereComponent* sphereCollision = FindComponentByClass<USphereComponent>();
	if (!IsValid(sphereCollision))
	{
		UE_LOG(LogTemp, Warning, TEXT("The sphere collision is invalid"));
		return;
	}
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnCollisionBeginOverlap);
	sphereCollision->OnComponentHit.AddDynamic(this, &ABall::OnCollisionBlock);
	
	UWidgetComponent* widgetComponent = FindComponentByClass<UWidgetComponent>();
	if (!IsValid(widgetComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::BeginPlay : no WidgetComponent for Direction Widget"));
		return;
	}
	
	directionWidget = widgetComponent;
	
	UStaticMeshComponent* meshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (!IsValid(meshComponent))
	{
		return;
	}
	
	ballMeshReference = meshComponent;
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// UEnum* EnumPtr = StaticEnum<EBallState>();
	// FString debugMessage = FString::Printf(TEXT("Ball state : %s"), *EnumPtr->GetNameStringByValue(static_cast<int64>(ballState)));
	// GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, debugMessage);
	

	if (ballState == Grabbed)
	{
		TickGrab(DeltaTime);
		return;
	}
	
	grabbedAnimationTimer = 0.f;
	
	if (ballState == Attracted)
	{
		TickAttract(DeltaTime);
		return;
	}
	
	if (ballState == Free)
	{
		if (!IsValid(ballEffectInstance))
		{
			UE_LOG(LogTemp, Warning, TEXT("ABall::Tick : The ballEffectInstance is invalid"));
			return;
		}
		
		ballEffectInstance->EffectTick(DeltaTime);
	}
}

void ABall::TickAttract(float DeltaTime)
{
	if (!IsValid(influenceSource))
	{
		UE_LOG(LogTemp, Warning, TEXT("The attraction source is invalid"));
		return;
	}
	
	FVector influenceSourceLocation;
	if (const AActor* influenceSourceActor = Cast<AActor>(influenceSource))
	{
		influenceSourceLocation = influenceSourceActor->GetActorLocation();
	}
	else if (const USceneComponent* sceneComp = Cast<USceneComponent>(influenceSource))
	{
		influenceSourceLocation = sceneComp->GetComponentLocation();
	}

	FVector newForwardVector = influenceSourceLocation - GetActorLocation();
	newForwardVector.Z = 0.f;
	SetActorRotation(newForwardVector.ToOrientationRotator());
}

void ABall::TickGrab(float DeltaTime)
{
	FVector influenceSourceLocation;
	FVector influenceSourceForwardVector;
	if (const AActor* influenceSourceActor = Cast<AActor>(influenceSource))
	{
		influenceSourceLocation = influenceSourceActor->GetActorLocation();
		influenceSourceForwardVector = influenceSourceActor->GetActorForwardVector();
	}
	else if (const USceneComponent* sceneComp = Cast<USceneComponent>(influenceSource))
	{
		influenceSourceLocation = sceneComp->GetComponentLocation();
		influenceSourceForwardVector = sceneComp->GetForwardVector();
	}
	
	// One of the three size values (they should all be equal anyway)
	double newSizeFactor = GetActorScale().X;
	newSizeFactor = FMath::Max(newSizeFactor - DeltaTime, grabAnimationSizeRatio);
	
	SetActorScale3D(newSizeFactor * FVector::OneVector);
	
	FVector goalLocation = influenceSourceLocation + FVector::CrossProduct(FVector::UpVector, influenceSourceForwardVector) * grabAnimDistance;
	
	float distanceToAttractionSource = FVector::Dist(GetActorLocation(), goalLocation);
	
	if (distanceToAttractionSource > minimumSpeed * 0.05f)
	{
		// Get the ball closer
		FVector newForwardVector = goalLocation - GetActorLocation();
		newForwardVector.Z = 0.0f;
		speed = FMath::Max(speed, minimumSpeed);
		SetActorRotation(newForwardVector.ToOrientationRotator());
		FVector actorLocation = GetActorLocation();
		actorLocation.Z = influenceSourceLocation.Z;
		SetActorLocation(actorLocation);
	}
	else
	{
		grabbedAnimationTimer = remainderf(grabbedAnimationTimer + (2 * PI * DeltaTime / grabAnimationCycleDuration), 2 * PI);
		
		float ballHeightDelta = grabAnimationHeight * FMath::Sin(grabbedAnimationTimer);
		
		
		SetActorLocation(goalLocation);
		ballMeshReference->SetRelativeLocation(ballHeightDelta * FVector::UpVector);
		
		speed = 0;
	}
	
}

EBallState ABall::GetBallState() const
{
	return ballState;
}

void ABall::SetCollisionEnabled(bool enabled) const
{
	auto* collisionComponent = GetComponentByClass<UShapeComponent>();

	if (!IsValid(collisionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall : No Collision Component found"))
		return;
	}
	
	FName newCollisionProfileName = enabled ? regularBallCollisionProfile : hollowBallCollisionProfile;
	
	collisionComponent->SetCollisionProfileName(newCollisionProfileName);
}

void ABall::OnCollisionBlock(UPrimitiveComponent* hitComponent, AActor* otherActor,
	UPrimitiveComponent* otherHitComponent, FVector normalImpulse, const FHitResult& hit)
{
	OnCollisionBlockBP(hitComponent, otherActor, otherHitComponent, normalImpulse, hit);
	
	if (!IsValid(ballEffectInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::OnCollisionBlock : The ballEffectInstance is invalid"));
		return;
	}
	
	if (!IsValid(otherActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::OnCollisionBlock : invalid otherActor"))
		return;
	}
	
	UClass* otherActorClass = Cast<UClass>(otherActor->GetClass());
	if (!IsValid(otherActorClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::OnCollisionBeginOverlap : invalid otherActor class"))
		return;
	}
	
	bool isDamageable = otherActorClass->ImplementsInterface(UDamageable::StaticClass());
		
	ballEffectInstance->CollisionBlock(hitComponent, otherActor, otherHitComponent, normalImpulse, hit, isDamageable);
}

void ABall::OnCollisionBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
                                    int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
	OnCollisionBeginOverlapBP(overlappedComponent, otherActor, otherComponent, otherBodyIndex, fromSweep, sweepResult);
	
	if (!IsValid(ballEffectInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::OnCollisionBeginOverlap : invalid ballEffectInstance"));
		return;
	}
	
	if (!IsValid(otherActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::OnCollisionBeginOverlap : invalid otherActor"))
		return;
	}
	
	UClass* otherActorClass = Cast<UClass>(otherActor->GetClass());
	if (!IsValid(otherActorClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::OnCollisionBeginOverlap : invalid otherActor class"))
		return;
	}

	const bool isDamageable = otherActorClass->ImplementsInterface(UDamageable::StaticClass());
	
	ballEffectInstance->CollisionBeginOverlap(overlappedComponent, otherActor, otherComponent, 
		otherBodyIndex, fromSweep, sweepResult, isDamageable);
}

void ABall::SetBallEffect(const TSubclassOf<ABallEffect> newBallEffectClass, bool actualize)
{
	if (!IsValid(newBallEffectClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::SetBallEffect : invalid newBallEffectClass"));
		return;
	}
	
	UWorld* world = GetWorld();
	
	if (!IsValid(world))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::SetBallEffect : invalid world"));
	}
	
	if (!actualize && newBallEffectClass == ballEffect)
	{
		return;
	}
	
	if (IsValid(ballEffectInstance))
	{
		ballEffectInstance->EffectRemoved();
		ballEffectInstance->Destroy();
	}
	
	ABallEffect* newBallEffectInstance = Cast<ABallEffect>(world->SpawnActor(newBallEffectClass));
	
	if (!IsValid(newBallEffectInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::SetBallEffect : could not spawn new ballEffectInstance"));
		return;
	}
	
	ballEffect = newBallEffectClass;
	ballEffectInstance = newBallEffectInstance;
	ballEffectInstance->effectOwner = this;

	ballEffectInstance->EffectApplied();
}

void ABall::BallHitByAttack(AActor* attacker)
{
	if (!IsValid(ballEffectInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::BallHitByAttack : no effect has been set"));
		return;
	}

	ballEffectInstance->Attack(attacker);
}

void ABall::UpdateDirectionWidgetHeight()
{
	FCollisionQueryParams collisionParameters;
	collisionParameters.AddIgnoredActor(this);
	
	UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		return;
	}
	
	// hacky float
	float directionWidgetMaxDepth = 150.f;
	
	FHitResult hitResult;
	world->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + directionWidgetMaxDepth * FVector::DownVector, ECC_Camera, collisionParameters);

	if (!hitResult.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(1012, 2.f, FColor::Red, TEXT("No hit !"));
		return;
	}
	
	directionWidget->SetWorldLocation(hitResult.ImpactPoint + FVector::UpVector);
	directionWidgetHeight = hitResult.ImpactPoint.Z + 1.f;
}

void ABall::ReleaseFromStationary(const float releaseSpeed)
{
	speed = releaseSpeed;
	
	if (IsValid(directionWidget))
	{
		directionWidget->SetVisibility(true);
	}

	const UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::ReleaseFromStationary : The world is invalid ?!"));
		return;
	}

	APlayerController* playerController = world->GetFirstPlayerController();
	
	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::ReleaseFromStationary : playerController is invalid"));
		return;
	}
	
	// TODO : Dirty way of getting the player character, do this properly (some day)
	ACharacter* playerCharacter = Cast<ACharacter>(playerController->GetPawn());
	
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::ReleaseFromStationary : playerCharacter is invalid"));
		return;
	}
	
	SetBallState(Attracted, playerCharacter);
}

void ABall::SetBallState(const EBallState newBallState, UObject* newInfluenceSource)
{
	UpdateDirectionWidgetHeight();
	ballMeshReference->SetRelativeLocation(FVector::Zero());
	
	if (newBallState == Free) 
	{
		SetActorScale3D(FVector::OneVector);
		SetCollisionEnabled(true);
		
		ballState = newBallState;
		if (IsValid(directionWidget))
		{
			directionWidget->SetVisibility(true);
		}
		return;
	}
	
	if (!IsValid(Cast<AActor>(newInfluenceSource)) && !IsValid(Cast<USceneComponent>(newInfluenceSource)))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall::SetBallState : invalid influenceSource, it should be an AActor or a SceneComponent"))
	}
	
	ballState = newBallState; 
	influenceSource = newInfluenceSource;
	
	if (newBallState == Grabbed)
	{
		UpdateDirectionWidgetHeight();
		SetCollisionEnabled(false);
		
		if (IsValid(directionWidget))
		{
			directionWidget->SetVisibility(false);
		}
		
		if (ABallContainer* ballContainer = Cast<ABallContainer>(influenceSource))
		{
			// TODO : edit this speed
			ballContainer->ReleaseBalls(600.f);
		}
		return;
	}
	
	if (newBallState == Attracted)
	{
		SetCollisionEnabled(true);
		
		if (IsValid(directionWidget))
		{
			directionWidget->SetVisibility(true);
		}
		return;
	}
	
	if (newBallState == Stationary)
	{
		SetActorScale3D(FVector::OneVector);
		SetCollisionEnabled(false);
		
		speed = 0;
		if (const AActor* influenceSourceActor = Cast<AActor>(influenceSource))
		{
			SetActorLocation(influenceSourceActor->GetActorLocation());
		}
		else if (const USceneComponent* sceneComp = Cast<USceneComponent>(influenceSource))
		{
			SetActorLocation(sceneComp->GetComponentLocation());
		}
		
		if (IsValid(directionWidget))
		{
			directionWidget->SetVisibility(false);
		}
		return;
	}
}



