#include "Earth.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"
#include "OrbitalInvaders/Core/OrbitalGameMode.h"

AEarth::AEarth()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(250.f);  
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = CollisionComponent;

	EarthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarthMesh"));
	EarthMesh->SetupAttachment(RootComponent);
	EarthMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEarth::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEarth::HandleOverlap);
	}
}

int32 AEarth::TakeDamage(int32 Amount)
{
	CurrentHealth = FMath::Max(0, CurrentHealth - Amount);
	UE_LOG(LogTemp, Warning, TEXT("Earth HP: %d/%d"), CurrentHealth, MaxHealth);
	
	if (CurrentHealth <= 0)
	{
		if (AOrbitalGameMode* GM = GetWorld()->GetAuthGameMode<AOrbitalGameMode>())
		{
			GM->TriggerGameOver(TEXT("Earth destroyed"));
		}
	}
	return CurrentHealth;
}

void AEarth::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		TakeDamage(1);
		Projectile->Destroy();
	}
}