#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

/**
 * Monitors when a wave is cleared 
 * cleans up the playfield, applies scaling,
 * and spawns the next wave after a short delay.
 *
 * Requires references to all spawner actors in the level.
 */
UCLASS()
class ORBITALINVADERS_API AWaveManager : public AActor
{
    GENERATED_BODY()

public:
    AWaveManager();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintPure, Category = "Wave")
    int32 GetCurrentWave() const { return CurrentWave; }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // ===== References to spawners (assigned in BP) =====

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|References")
    TObjectPtr<class AInvaderFormationManager> FormationManager;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|References")
    TObjectPtr<class AAsteroidSpawner> AsteroidSpawner;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|References")
    TObjectPtr<class ABunkerSpawner> BunkerSpawner;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|References")
    TObjectPtr<class ASpecialInvaderSpawner> SpecialInvaderSpawner;

    // Configuration

    /** Delay between wave clear and next wave spawn (seconds). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    float WaveDelay = 3.f;

    // Scaling per wave

    /** Multiplier applied to formation rotation speed each wave. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Scaling")
    float RotationSpeedScale = 1.1f;

    /** Multiplier applied to invader fire interval each wave (< 1.0 = faster). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Scaling")
    float FireIntervalScale = 0.9f;

    /** Multiplier applied to formation jump interval each wave (< 1.0 = faster jumps). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Scaling")
    float JumpIntervalScale = 0.9f;

    /** Multiplier applied to asteroid spawn interval each wave (< 1.0 = more asteroids). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Scaling")
    float AsteroidSpawnScale = 0.85f;

    /** Multiplier applied to special invader's radial speed each wave (> 1.0 = faster descent). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Scaling")
    float SpecialInvaderSpeedScale = 1.15f;

private:
    int32 CurrentWave = 1;
    bool bWaitingForNextWave = false;
    float WaveDelayTimer = 0.f;

    UFUNCTION()                              
    void OnFormationCleared();

    /** Clean the playfield between waves. */
    void CleanupPlayfield();

    /** Spawn next wave with scaled difficulty. */
    void StartNextWave();

    /** Apply scaling multipliers to all spawners. */
    void ApplyWaveScaling();
};