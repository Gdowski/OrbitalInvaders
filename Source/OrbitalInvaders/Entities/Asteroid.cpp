// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "Bunker.h"
#include "Earth.h"
#include "Invader.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "OrbitalInvaders/Systems/AsteroidSpawner.h"
#include "OrbitalInvaders/Systems/VFXHelper.h"

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = false; 

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(80.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Asteroid"));
	RootComponent = CollisionComponent;

	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	AsteroidMesh->SetupAttachment(RootComponent);
	AsteroidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->ProjectileGravityScale = 0.f;  
	MovementComponent->bRotationFollowsVelocity = false;  
	MovementComponent->bShouldBounce = false;
	MovementComponent->InitialSpeed = 0.f;  // we set velocity via Init()
	MovementComponent->MaxSpeed = 5000.f;   // for safety ceiling
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::HandleOverlap);
	}
}


void AAsteroid::Init(const FVector& InVelocity, EAsteroidSize InSize)
{
	Size = InSize;
	Velocity = InVelocity;

	if (MovementComponent)
	{
		MovementComponent->Velocity = InVelocity;
	}

	float Scale = 1.f;
	float Radius = 80.f;

	switch (Size)
	{
		case EAsteroidSize::Large:  Scale = 1.5f; Radius = 120.f; break;
		case EAsteroidSize::Medium: Scale = 1.0f; Radius = 80.f;  break;
		case EAsteroidSize::Small:  Scale = 0.6f; Radius = 50.f;  break;
	}

	if (AsteroidMesh)
	{
		AsteroidMesh->SetRelativeScale3D(FVector(Scale));
	}
	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(Radius);
	}
}

void AAsteroid::SplitOrDestroy()
{
	
	if (bIsBeingDestroyed) return;
	bIsBeingDestroyed = true;
	// Small asteroids are destroyed outright
	if (Size == EAsteroidSize::Small)
	{
		Destroy();
		return;
	}

	// Determine next size down
	EAsteroidSize NextSize = (Size == EAsteroidSize::Large)
		? EAsteroidSize::Medium
		: EAsteroidSize::Small;

	UWorld* World = GetWorld();
	if (!World)
	{
		Destroy();
		return;
	}

	// Spawn 2 smaller asteroids with perpendicular spread
	const FVector Location = GetActorLocation();
	const FVector OriginalDirection = Velocity.GetSafeNormal();
	const float OriginalSpeed = Velocity.Size();

	// Perpendicular vector in XY plane
	const FVector Perpendicular(-OriginalDirection.Y, OriginalDirection.X, 0.f);
	constexpr float SpreadAngle = 0.5f;

	// Two new velocities: original +/- perpendicular component
	const FVector VelA = (OriginalDirection + Perpendicular * SpreadAngle).GetSafeNormal() * OriginalSpeed;
	const FVector VelB = (OriginalDirection - Perpendicular * SpreadAngle).GetSafeNormal() * OriginalSpeed;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Deferred spawn: construct actor but don't call BeginPlay yet.
	// This lets us set fields (Size, Velocity) before overlap handlers activate.
	// Avoided stack overflow while coliding with an object
	const FTransform ChildTransform(FRotator::ZeroRotator, Location);
	AAsteroid* ChildA = World->SpawnActorDeferred<AAsteroid>(GetClass(), ChildTransform, GetOwner(), nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn );
	if (ChildA)
	{
		ChildA->Init(VelA, NextSize);
		ChildA->FinishSpawning(ChildTransform);
	}

	AAsteroid* ChildB = World->SpawnActorDeferred<AAsteroid>(GetClass(), ChildTransform, GetOwner(), nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn );
	if (ChildB)
	{
		ChildB->Init(VelB, NextSize);
		ChildB->FinishSpawning(ChildTransform);
	}

	if (AAsteroidSpawner* Spawner = Cast<AAsteroidSpawner>(GetOwner()))
	{
		if (ChildA) Spawner->RegisterAsteroid(ChildA);
		if (ChildB) Spawner->RegisterAsteroid(ChildB);
	}
	if (ExplosionEffect)
	{
		const float VFXScale = (Size == EAsteroidSize::Large) ? 2.f
							 : (Size == EAsteroidSize::Medium) ? 1.5f
							 : 1.f;
		UVFXHelper::SpawnExplosion(this, ExplosionEffect, GetActorLocation(), VFXScale);
	}
	Destroy();
}

void AAsteroid::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	if (bIsBeingDestroyed) return; 
	
	UE_LOG(LogTemp, Warning, TEXT("Asteroid hit by: %s (class: %s)"),
		*OtherActor->GetName(),
		*OtherActor->GetClass()->GetName());
	// Hit by projectile - split
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		if (Projectile->IsPlayerProjectile())
		{
			if (AOrbitalGameState* GS = GetWorld()->GetGameState<AOrbitalGameState>())
			{
				GS->AddScoreFor(GetScoreEventForSize());
			}
		}
		Projectile->Destroy();
		SplitOrDestroy();
		return;
	}
	
	// Hit an invader - both die, asteroid splits
	if (OtherActor->IsA(AInvader::StaticClass()))
	{
		SplitOrDestroy();
		return;
	}
	
	// Hit a bunker - asteroid splits 
	if (OtherActor->IsA(ABunker::StaticClass()))
	{
		SplitOrDestroy();
		return;
	}
	
	// Hit player - destroy asteroid (no split)
	if (OtherActor->IsA(APlayerShip::StaticClass()))
	{
		Destroy();
		return;
	}
	
	// Hit Earth - destroy asteroid
	if (OtherActor->IsA(AEarth::StaticClass()))
	{
		Destroy();
		return;
	}
}

EScoreEvent AAsteroid::GetScoreEventForSize() const
{
	switch (Size)
	{
	case EAsteroidSize::Small:  return EScoreEvent::AsteroidSmallHit;
	case EAsteroidSize::Medium: return EScoreEvent::AsteroidMediumHit;
	case EAsteroidSize::Large:  return EScoreEvent::AsteroidLargeHit;
	}
	return EScoreEvent::AsteroidSmallHit;  // fallback
}