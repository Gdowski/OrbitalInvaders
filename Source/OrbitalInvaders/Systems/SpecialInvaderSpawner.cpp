#include "SpecialInvaderSpawner.h"

#include "OrbitalInvaders/Entities/SpecialInvader.h"


ASpecialInvaderSpawner::ASpecialInvaderSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpecialInvaderSpawner::BeginPlay()
{
	Super::BeginPlay();
	TimeSinceLastSpawn = 0.f;
}

void ASpecialInvaderSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;

	const float RequiredDelay = bFirstSpawnDone ? SpawnInterval : InitialDelay;

	if (TimeSinceLastSpawn >= RequiredDelay)
	{
		TimeSinceLastSpawn = 0.f;
		bFirstSpawnDone = true;
		SpawnSpecialInvader();
	}
}

void ASpecialInvaderSpawner::SpawnSpecialInvader()
{
	if (!SpecialInvaderClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// SpecialInvader computes its own location in BeginPlay + Tick, so spawn at origin
	ActiveSpecialInvader = World->SpawnActor<ASpecialInvader>(
		SpecialInvaderClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);
	// Apply wave scaling to movement speed
	if (ActiveSpecialInvader)
	{
		ActiveSpecialInvader->SetSpeedMultiplier(SpeedMultiplier);
	}
}
void ASpecialInvaderSpawner::ClearAndReset()
{
	if (IsValid(ActiveSpecialInvader))
	{
		ActiveSpecialInvader->Destroy();
	}
	ActiveSpecialInvader = nullptr;
	TimeSinceLastSpawn = 0.f;
	bFirstSpawnDone = false;
}