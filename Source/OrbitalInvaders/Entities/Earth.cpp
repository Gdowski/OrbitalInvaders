#include "Earth.h"

#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"
#include "OrbitalInvaders/Core/OrbitalGameMode.h"
#include "OrbitalInvaders/Core/OrbitalPlayerController.h"
#include "OrbitalInvaders/Systems/VFXHelper.h"

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

int32 AEarth::ApplyDamage(int32 Amount)
{
	CurrentHealth = FMath::Max(0, CurrentHealth - Amount);
	UE_LOG(LogTemp, Warning, TEXT("Earth HP: %d/%d"), CurrentHealth, MaxHealth);
	if (HitSound)
	{
		UVFXHelper::PlaySFX2D(this, HitSound);
	}
	
	// Screen shake + HUD update
	if (AOrbitalPlayerController* PC = AOrbitalPlayerController::Get(this))
	{
		PC->PlayCameraShake(0.1f);
		if (UOrbitalHUD* HUD = PC->GetOrbitalHUD())
		{
			HUD->UpdateEarthHealth(CurrentHealth);
		}
	}
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
		ApplyDamage(1);
		Projectile->Destroy();
		return;
	}

	// Asteroid hit — instant game over per game spec
	if (Cast<AAsteroid>(OtherActor))
	{
		if (AOrbitalGameMode* GM = GetWorld()->GetAuthGameMode<AOrbitalGameMode>())
		{
			GM->TriggerGameOver(TEXT("Asteroid impacted Earth"));
		}
		return;
	}
}