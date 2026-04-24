#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bunker.generated.h"

/**
 * ABunker - destructible shield between player and invaders.
 *
 * Takes damage from invader and player projectiles, and from asteroids.
 * Visually transitions through damage states before being destroyed.
 */
UCLASS()
class ORBITALINVADERS_API ABunker : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    ABunker();

    // Public API
    UFUNCTION(BlueprintCallable, Category = "Combat")
    int32 ApplyDamage(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Combat")
    int32 GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    int32 GetMaxHealth() const { return MaxHealth; }

protected:
    // Virtual overrides
    virtual void BeginPlay() override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UBoxComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UStaticMeshComponent> BunkerMesh;

    // Config
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 MaxHealth = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
    TArray<TObjectPtr<class UStaticMesh>> DamageStateMeshes;

    // SFX / VFX
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
    TObjectPtr<class USoundBase> HitSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
    TObjectPtr<class UNiagaraSystem> DestructionEffect;

    // Runtime state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 CurrentHealth = 5;

    // Virtual hooks
    void OnDeath();

    UFUNCTION(BlueprintCallable, Category = "Visuals")
    void UpdateDamageVisual();

private:
    // Overlap callbacks
    UFUNCTION()
    void HandleOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};