#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

/**
 * AAsteroidSpawner - spawns asteroids along the edge of the playfield
 * and handles wrap-around behavior for existing asteroids.
 */
UCLASS()
class ORBITALINVADERS_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()

public:
    // Constructor
    AAsteroidSpawner();

    // Public API
    /** Called by child asteroids after split to register them. */
    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void RegisterAsteroid(class AAsteroid* Asteroid);

    void ClearAllAsteroids();
    float GetSpawnInterval() const { return SpawnInterval; }
    void SetSpawnInterval(float NewInterval) { SpawnInterval = NewInterval; }

protected:
    // Virtual overrides
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Config
    /** Asteroid class to spawn. Assign in the BP. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    TSubclassOf<class AAsteroid> AsteroidClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float SpawnInterval = 2.5f;

    /** Half-size of the spawn box. Asteroids spawn just outside this area. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float SpawnAreaHalfExtent = 3000.f;

    /** Half-size of the playfield. Asteroids beyond this wrap to the opposite side. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float PlayfieldHalfExtent = 3500.f;

    /** Minimum asteroid speed. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float MinSpeed = 400.f;

    /** Maximum asteroid speed. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float MaxSpeed = 1200.f;

private:
    // Internal helpers
    void SpawnAsteroid();
    void UpdateWrapAround();

    // Runtime state
    UPROPERTY()
    TArray<TObjectPtr<class AAsteroid>> SpawnedAsteroids;

    float TimeSinceLastSpawn = 0.f;
};