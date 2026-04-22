// Fill out your copyright notice in the Description page of Project Settings.


#include "Invader.h"

#include "Projectile.h"
#include "Bunker.h"

#include "Components/SphereComponent.h"

// Sets default values
AInvader::AInvader()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Collision sphere
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(40.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Invader"));
	RootComponent = CollisionComponent;
	
	// Mesh
	InvaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvaderMesh"));
	InvaderMesh->SetupAttachment(RootComponent);
	InvaderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AInvader::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Invader BeginPlay: CollisionComponent is %s"),
	   CollisionComponent ? TEXT("VALID") : TEXT("NULL"));
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,&AInvader::HandleOverlap);
		UE_LOG(LogTemp, Warning, TEXT("Invader BeginPlay: bound=%d"),
		CollisionComponent->OnComponentBeginOverlap.IsBound());
	}
	
}

void AInvader::SetOrbitalPosition(float Angle, float OrbitRadius)
{
	const float X = FMath::Cos(Angle) * OrbitRadius;
	const float Y = FMath::Sin(Angle) * OrbitRadius;
	SetActorLocation(FVector(X, Y, 0.f));

	// Face Earth (center of the world)
	const FVector Direction = (-GetActorLocation()).GetSafeNormal();
	SetActorRotation(Direction.Rotation());
}

bool AInvader::TakeDamage(int32 Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0)
	{
		OnDeath();
		return true;
	}
	return false;
}

void AInvader::OnDeath()
{
	Destroy();
}

void AInvader::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Invader HandleOverlap: OtherActor=%s"),
	OtherActor ? *OtherActor->GetName() : TEXT("NULL"));
	if (!OtherActor || OtherActor == this) return;
	// Check if it's a player projectile
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		TakeDamage(1);
		Projectile->Destroy();
	}
 	// Collided with bunker - destroy both
	if (ABunker* Bunker = Cast<ABunker>(OtherActor))
	{
		Bunker->TakeDamage(Bunker->GetMaxHealth());
		OnDeath();
	}
}

void AInvader::Fire()
{
	if (!ProjectileClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	// Fire direction: toward Earth (center of world)
	const FVector FireDirection = (-GetActorLocation()).GetSafeNormal();
	const FVector SpawnLocation = GetActorLocation() + FireDirection * ProjectileSpawnOffset;
	const FRotator SpawnRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Init(FireDirection);
	}
}
