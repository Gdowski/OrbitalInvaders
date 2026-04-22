// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

#include "OrbitalInvaders/Entities/Asteroid.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= SpawnInterval)
	{
		TimeSinceLastSpawn = 0.f;
		SpawnAsteroid();
	}

	UpdateWrapAround();
}


void AAsteroidSpawner::SpawnAsteroid()
{
	if (!AsteroidClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	// Pick a random edge (0 = top, 1 = right, 2 = bottom, 3 = left)
	const int32 Edge = FMath::RandRange(0, 3);
	const float Offset = FMath::RandRange(-SpawnAreaHalfExtent, SpawnAreaHalfExtent);

	FVector SpawnLocation;
	switch (Edge)
	{
	case 0: SpawnLocation = FVector(SpawnAreaHalfExtent, Offset, 0.f); break;   // top (+X)
	case 1: SpawnLocation = FVector(Offset, SpawnAreaHalfExtent, 0.f); break;   // right (+Y)
	case 2: SpawnLocation = FVector(-SpawnAreaHalfExtent, Offset, 0.f); break;  // bottom (-X)
	case 3: SpawnLocation = FVector(Offset, -SpawnAreaHalfExtent, 0.f); break;  // left (-Y)
	}

	// Fully random direction
	const float DirAngle = FMath::RandRange(0.f, 2.f * PI);
	const FVector Direction(FMath::Cos(DirAngle), FMath::Sin(DirAngle), 0.f);

	const float Speed = FMath::RandRange(MinSpeed, MaxSpeed);
	const FVector Velocity = Direction * Speed;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAsteroid* Asteroid = World->SpawnActor<AAsteroid>(
		AsteroidClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (Asteroid)
	{
		const float Roll = FMath::FRand();
		EAsteroidSize Size = EAsteroidSize::Large;
		if (Roll > 0.9f) Size = EAsteroidSize::Small;
		else if (Roll > 0.6f) Size = EAsteroidSize::Medium;

		Asteroid->Init(Velocity, Size);
		SpawnedAsteroids.Add(Asteroid);
	}
}

void AAsteroidSpawner::UpdateWrapAround()
{
	// Clean up destroyed asteroids
	SpawnedAsteroids.RemoveAll([](const TObjectPtr<AAsteroid>& A)
	{
		return !IsValid(A);
	});

	for (AAsteroid* Asteroid : SpawnedAsteroids)
	{
		if (!Asteroid) continue;

		FVector Loc = Asteroid->GetActorLocation();
		bool bWrapped = false;

		if (Loc.X > PlayfieldHalfExtent)       { Loc.X = -PlayfieldHalfExtent; bWrapped = true; }
		else if (Loc.X < -PlayfieldHalfExtent) { Loc.X = PlayfieldHalfExtent;  bWrapped = true; }

		if (Loc.Y > PlayfieldHalfExtent)       { Loc.Y = -PlayfieldHalfExtent; bWrapped = true; }
		else if (Loc.Y < -PlayfieldHalfExtent) { Loc.Y = PlayfieldHalfExtent;  bWrapped = true; }

		if (bWrapped)
		{
			Asteroid->SetActorLocation(Loc);
		}
	}
}

void AAsteroidSpawner::RegisterAsteroid(AAsteroid* Asteroid)
{
	if (Asteroid)
	{
		SpawnedAsteroids.AddUnique(Asteroid);
	}
}
