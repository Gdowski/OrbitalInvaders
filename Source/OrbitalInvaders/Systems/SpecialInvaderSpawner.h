#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialInvaderSpawner.generated.h"

/**
 * ASpecialInvaderSpawner - periodically spawns a special invader that
 * spirals toward Earth independently of the main formation.
 */
UCLASS()
class ORBITALINVADERS_API ASpecialInvaderSpawner : public AActor
{
	GENERATED_BODY()
	
public:
    // Constructor
    ASpecialInvaderSpawner();

    // Public API
    void ClearAndReset();
    float GetSpeedMultiplier() const { return SpeedMultiplier; }
    void SetSpeedMultiplier(float NewMultiplier) { SpeedMultiplier = NewMultiplier; }

protected:
    // Virtual overrides
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Config
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    TSubclassOf<class ASpecialInvader> SpecialInvaderClass;

    /** Time between spawns (seconds). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float SpawnInterval = 15.f;

    /** Delay before first spawn after level start (seconds). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float InitialDelay = 10.f;

private:
    // Internal helpers
    void SpawnSpecialInvader();

    // Runtime state
    UPROPERTY()
    TObjectPtr<class ASpecialInvader> ActiveSpecialInvader;

    float SpeedMultiplier = 1.f;
    float TimeSinceLastSpawn = 0.f;
    bool bFirstSpawnDone = false;
};