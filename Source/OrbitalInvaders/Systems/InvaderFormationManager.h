// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvaderFormationManager.generated.h"

/** Broadcast when no invaders. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFormationCleared);

/**
 * AInvaderFormationManager - spawns and orchestrates the invader formation.
 */
UCLASS()
class ORBITALINVADERS_API AInvaderFormationManager : public AActor
{
    GENERATED_BODY()

public:
    AInvaderFormationManager();

    virtual void Tick(float DeltaTime) override;
    
    /** Returns number of living invaders. */
    UFUNCTION(BlueprintPure, Category = "Formation")
    int32 GetLivingInvaderCount() const { return Invaders.Num(); }
    
    void SpawnFormation();
    void ClearFormation();
    void ResetFormation();

    // Getters/setters for wave scaling
    float GetRotationSpeed() const { return RotationSpeed; }
    void SetRotationSpeed(float NewSpeed) { RotationSpeed = NewSpeed; }
    float GetFireInterval() const { return FireInterval; }
    void SetFireInterval(float NewInterval) { FireInterval = NewInterval; }
    float GetJumpInterval() const { return JumpInterval; }
    void SetJumpInterval(float NewInterval) { JumpInterval = NewInterval; }
    float GetInvadersPerOrbit() const { return InvadersPerOrbit; }
    void SetInvadersPerOrbit(float NewInvadersPerOrbit) { InvadersPerOrbit = NewInvadersPerOrbit; }
    float GetOrbitCount() const { return OrbitCount; }
    void SetOrbitCount(float NewOrbitCount) { OrbitCount = NewOrbitCount; }
    
    /** Event when all invaders are destroyed. For WaveManager. */
    UPROPERTY(BlueprintAssignable, Category = "Formation")
    FOnFormationCleared OnFormationCleared;

protected:
    virtual void BeginPlay() override;

    // Configuration 

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
    
    bool bFormationClearedBroadcasted = false;
    
    // Runtime state
    float TimeSinceLastFire = 0.f;
    
    /** Current global rotation offset of the formation (radians). */
    float FormationAngle = 0.f;

    float FormationRadialOffset = 0.f;

    /** Time accumulated since last jump. */
    float TimeSinceLastJump = 0.f;
    
    float TimeSinceLastReversal = 0.f;
    /** Current direction multiplier: +1 or -1. */
    float RotationDirection = 1.f;

    /** Live invaders currently in the formation. */
    UPROPERTY()
    TArray<TObjectPtr<class AInvader>> Invaders;

    // Internal methods
    


    void PerformJump();

    void UpdateInvaderPositions();

    float GetOrbitRadius(int32 OrbitIndex) const;
    
    void FireRandomInvader();
};