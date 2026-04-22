#include "Bunker.h"

#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"

ABunker::ABunker()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(60.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Bunker"));
    RootComponent = CollisionComponent;

    BunkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BunkerMesh"));
    BunkerMesh->SetupAttachment(RootComponent);
    BunkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABunker::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;

    if (CollisionComponent)
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABunker::HandleOverlap);
    }

    UpdateDamageVisual();
}

int32 ABunker::ApplyDamage(int32 Amount)
{
    CurrentHealth = FMath::Max(0, CurrentHealth - Amount);

    if (CurrentHealth <= 0)
    {
        OnDeath();
    }
    else
    {
        UpdateDamageVisual();
    }

    return CurrentHealth;
}

void ABunker::OnDeath()
{
    Destroy();
}

void ABunker::UpdateDamageVisual()
{
    if (DamageStateMeshes.Num() == 0 || !BunkerMesh) return;

    // Map HP to mesh index
    // HP = Max - index 0 (pristine), HP = 1 - last index (most damaged)
    const float HealthFraction = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
    const int32 StateCount = DamageStateMeshes.Num();

    // Invert: high HP = low index, low HP = high index
    const int32 StateIndex = FMath::Clamp(
        StateCount - 1 - FMath::FloorToInt(HealthFraction * StateCount),
        0,
        StateCount - 1
    );

    if (DamageStateMeshes[StateIndex])
    {
        BunkerMesh->SetStaticMesh(DamageStateMeshes[StateIndex]);
    }
}

void ABunker::HandleOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    // Hit by projectile
    if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
    {
        ApplyDamage(1);
        Projectile->Destroy();
        return;
    }

    // Hit by asteroid — bunker takes damage, asteroid splits itself
    if (Cast<AAsteroid>(OtherActor))
    {
        ApplyDamage(1);
        return;
    }
}