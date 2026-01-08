// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/Ball.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameLogic/GameplayComponents/MagnetComponent.h"
#include "GameLogic/Interfaces/Damageable.h"

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
	
	USphereComponent* SphereCollision = FindComponentByClass<USphereComponent>();
	if (!IsValid(SphereCollision))
	{
		UE_LOG(LogTemp, Warning, TEXT("The sphere collision is invalid"));
		return;
	}
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnCollision);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ballState == Attracted)
	{
		TickAttract();
		return;
	}

	if (ballState == Grabbed)
	{
		TickGrab();
		return;
	}
	
	if (ballState == Free)
	{
		// There might be a nicer way to do this, but I'm unsure of the best solution. This will work for now
		// The CDO is not supposed to get instanced at each call 
		
		if (!IsValid(ballEffect))
		{
			UE_LOG(LogTemp, Warning, TEXT("The effect is invalid"));
			return;
		}
		
		UBallEffect* defaultObject = Cast<UBallEffect>(ballEffect.Get()->GetDefaultObject());
		
		if (!IsValid(defaultObject))
		{
			UE_LOG(LogTemp, Warning, TEXT("The default object is NULL"));
			return;
		}
		
		defaultObject->Tick(DeltaTime, this);
	}
}

void ABall::TickAttract()
{
	if (!IsValid(attractionSource))
	{
		UE_LOG(LogTemp, Warning, TEXT("The attraction source is invalid"));
		return;
	}

	FVector newForwardVector = attractionSource->GetActorLocation() - GetActorLocation();
	SetActorRotation(newForwardVector.ToOrientationRotator());
}

void ABall::TickGrab()
{
	float distanceToAttractionSouce = FVector::Dist(GetActorLocation(), attractionSource->GetActorLocation());
	FVector newForwardVector;

	// TODO : add smoother transition between the three cases if needed
	if (distanceToAttractionSouce > grabAnimDistance + epsilonDistance)
	{
		// Get the ball closer
		newForwardVector = attractionSource->GetActorLocation() - GetActorLocation();
	}
	else if (distanceToAttractionSouce < grabAnimDistance - epsilonDistance)
	{
		// Get the ball further
		newForwardVector = GetActorLocation() - attractionSource->GetActorLocation();
	}
	else
	{
		// Rotate ball clockwise
		newForwardVector = FVector::CrossProduct(FVector::UpVector, attractionSource->GetActorLocation() - GetActorLocation());
	}

	SetActorRotation(newForwardVector.ToOrientationRotator());
}

void ABall::SetNewAttractionSource(const AActor* newAttractionSource)
{
	SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = newAttractionSource;
	ballState = Attracted;
}

void ABall::SetNewGrabSource(const AActor* newGrabSource)
{
	// TODO : fix collisions 
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	attractionSource = newGrabSource;
	ballState = Grabbed; 
}

EBallState ABall::GetBallState() const
{
	return ballState;
}

void ABall::Explode(UNiagaraSystem* explosionParticleSystem, float explosionRange, float explosionDamage)
{
	TArray<FHitResult> attackHitResults;
	const auto attackCollisionShape = FCollisionShape::MakeSphere(explosionRange);
	
	// TODO : Allow this to be changed from editor
	constexpr ECollisionChannel attackTraceChannel = ECC_GameTraceChannel3;
	
	// TODO : Test for collision during the whole animation
	GetWorld()->SweepMultiByChannel(attackHitResults, 
		GetActorLocation(), GetActorLocation(), 
		FQuat::Identity, attackTraceChannel, attackCollisionShape);
	
	if (attackHitResults.IsEmpty())
	{
		// Attack did not hit a target
		return;
	}
	
	for (FHitResult hitResult : attackHitResults)
	{
		auto* hitActor = Cast<AActor>(hitResult.GetActor());
		if (!hitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
		{
			continue;
		}
	
		IDamageable::Execute_ReceiveDamage(hitActor, explosionDamage, hitResult.ImpactNormal, this);
	}
	
	// Spawn Niagara System
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionParticleSystem, GetActorLocation(), FRotator::ZeroRotator,
		FVector(0.4));
	
	// TODO : do this properly
	// Get player character
	APawn* playerCharacter = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(playerCharacter))
	{
		return;
	}
	
	UMagnetComponent* magnetComponent = playerCharacter->GetComponentByClass<UMagnetComponent>();
	
	if (!IsValid(magnetComponent))
	{
		return;
	}
	
	// Set attracted object
	magnetComponent->SetAttractedObject(this);
	
	// Grab attracted object
	magnetComponent->GrabAttractedObject();
	
	SetActorLocation(playerCharacter->GetActorLocation());
}

void ABall::SetCollisionEnabled(ECollisionEnabled::Type collisionType) const
{
	auto* collisionComponent = GetComponentByClass<UShapeComponent>();

	if (!IsValid(collisionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("ABall : No Collision Component found"))
		return;
	}

	collisionComponent->SetCollisionEnabled(collisionType);
}

void ABall::OnCollision(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
	OnCollisionBP(overlappedComponent, otherActor, otherComponent, otherBodyIndex, fromSweep, sweepResult);
	
	if (!IsValid(ballEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("The ball effect is invalid"));
		return;
	}
		
	UBallEffect* defaultObject = Cast<UBallEffect>(ballEffect.Get()->GetDefaultObject());
		
	if (!IsValid(defaultObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("The default object is NULL"));
		return;
	}
		
	defaultObject->Collide(this, overlappedComponent, otherActor, otherComponent, otherBodyIndex, fromSweep, sweepResult);
}

void ABall::FreeFromAttraction()
{
	if (ballState != Grabbed && ballState != Attracted)
	{
		return;
	}
	
	SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	attractionSource = nullptr;
	ballState = Free;
}

void ABall::SetStationaryAtLocation(const FVector& location)
{
	SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	attractionSource = nullptr;
	ballState = Stationary;
	speed = 0;
	
	// TODO : Add animation for ball transition to new location
	SetActorLocation(location);
	
	directionWidget->SetVisibility(false);
}

void ABall::ReleaseFromStationary(float releaseSpeed)
{
	speed = releaseSpeed;
	
	directionWidget->SetVisibility(true);
	
	UWorld* world = GetWorld();
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
	
	// Dirty way of getting the player character
	ACharacter* playerCharacter = Cast<ACharacter>(playerController->GetPawn());
	
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABall::ReleaseFromStationary : playerCharacter is invalid"));
		return;
	}
	
	SetNewGrabSource(playerCharacter);
}



