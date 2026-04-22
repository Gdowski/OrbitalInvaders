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
    ABunker();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    int32 TakeDamage(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Combat")
    int32 GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    int32 GetMaxHealth() const { return MaxHealth; }

protected:
    virtual void BeginPlay() override;

    //  Components 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UStaticMeshComponent> BunkerMesh;

    //  Health 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 MaxHealth = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 CurrentHealth = 5;

    //  Visual damage states 

    /** Meshes for different damage levels (0 = pristine, last = most damaged). Assigned in BP. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
    TArray<TObjectPtr<class UStaticMesh>> DamageStateMeshes;

    UFUNCTION(BlueprintCallable, Category = "Visuals")
    void UpdateDamageVisual();

    void OnDeath();

private:
    UFUNCTION()
    void HandleOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};