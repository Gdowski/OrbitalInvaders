#include "BunkerSpawner.h"
#include "OrbitalInvaders/Entities/Bunker.h"

ABunkerSpawner::ABunkerSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABunkerSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!BunkerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("BunkerSpawner: BunkerClass is not set"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	const float AngleStep = 2.f * PI / static_cast<float>(BunkerCount);

	for (int32 i = 0; i < BunkerCount; ++i)
	{
		const float Angle = AngleStep * static_cast<float>(i);
		const float X = FMath::Cos(Angle) * BunkerOrbitRadius;
		const float Y = FMath::Sin(Angle) * BunkerOrbitRadius;

		const FVector SpawnLocation(X, Y, 0.f);

		// Bunker faces Earth (center)
		const FVector DirectionToCenter = (-SpawnLocation).GetSafeNormal();
		const FRotator SpawnRotation = DirectionToCenter.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		World->SpawnActor<ABunker>(BunkerClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}