// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision sphere 
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);  // enable OnHit events
	RootComponent = CollisionComponent;

	// Mesh 
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Movement component 
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = ProjectileSpeed;
	MovementComponent->MaxSpeed = ProjectileSpeed;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.f;  // no gravity in space

	// Auto-destroy after LifeSpan seconds
	InitialLifeSpan = LifeSpan;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Init(const FVector& Direction)
{
	if (MovementComponent)
	{
		MovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;
	}
}

