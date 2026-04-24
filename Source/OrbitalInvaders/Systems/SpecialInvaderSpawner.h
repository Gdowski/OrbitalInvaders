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
    AInvaderFormationManager();

    // Public API
    /** Returns number of living invaders. */
    UFUNCTION(BlueprintPure, Category = "Formation")
    int32 GetLivingInvaderCount() const { return Invaders.Num(); }

    void SpawnFormation();
    void ClearFormation();
    void ResetFormation();

    float GetRotationSpeed() const { return RotationSpeed; }
    void SetRotationSpeed(float NewSpeed) { RotationSpeed = NewSpeed; }
    float GetFireInterval() const { return FireInterval; }
    void SetFireInterval(float NewInterval) { FireInterval = NewInterval; }
    float GetJumpInterval() const { return JumpInterval; }
    void SetJumpInterval(float NewInterval) { JumpInterval = NewInterval; }
    int32 GetInvadersPerOrbit() const { return InvadersPerOrbit; }
    void SetInvadersPerOrbit(int32 NewInvadersPerOrbit) { InvadersPerOrbit = NewInvadersPerOrbit; }
    int32 GetOrbitCount() const { return OrbitCount; }
    void SetOrbitCount(int32 NewOrbitCount) { OrbitCount = NewOrbitCount; }

    // Delegates
    /** Event when all invaders are destroyed. For WaveManager. */
    UPROPERTY(BlueprintAssignable, Category = "Formation")
    FOnFormationCleared OnFormationCleared;

protected:
    // Virtual overrides
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Config
    /** Class used to spawn each invader. Assign in the BP. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    TSubclassOf<class AInvader> InvaderClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation", meta = (ClampMin = "1"))
    int32 OrbitCount = 6;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation", meta = (ClampMin = "1"))
    int32 InvadersPerOrbit = 8;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float InnermostRadius = 1400.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float OrbitSpacing = 200.f;

    /** Angular speed of the formation rotation (radians per second). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float RotationSpeed = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float JumpInterval = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float JumpDistance = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float RotationReversalInterval = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float FireInterval = 1.5f;

    /** Radius below which the formation triggers Game Over (should match player's orbit). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
    float GameOverRadius = 800.f;

private:
    // Internal helpers
    void PerformJump();
    void UpdateInvaderPositions();
    void FireRandomInvader();
    float GetOrbitRadius(int32 OrbitIndex) const;

    // Runtime state
    UPROPERTY()
    TArray<TObjectPtr<class AInvader>> Invaders;

    float TimeSinceLastFire = 0.f;
    float FormationAngle = 0.f;
    float FormationRadialOffset = 0.f;
    float TimeSinceLastJump = 0.f;
    float TimeSinceLastReversal = 0.f;
    float RotationDirection = 1.f;
    bool bFormationClearedBroadcasted = false;
};