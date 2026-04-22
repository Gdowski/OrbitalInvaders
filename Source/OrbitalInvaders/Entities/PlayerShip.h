// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShip.generated.h"

/**
 * APlayerShip - Player-controlled ship orbiting Earth.
 *
 * Blueprint subclass (BP_PlayerShip) is expected to provide:
 *   - Static Mesh on ShipMesh component
 *   - MoveAction     (reference to IA_Move)
 *   - FireAction     (reference to IA_Fire)
 *   - ProjectileClass (reference to BP_PlayerProjectile or subclass)
 */
UCLASS()
class ORBITALINVADERS_API APlayerShip : public APawn
{
    GENERATED_BODY()

public:
    APlayerShip();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    int32 ApplyDamage(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Health")
    int32 GetCurrentHealth() const { return CurrentHealth; }

protected:
    virtual void BeginPlay() override;

    //  Components 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UStaticMeshComponent> ShipMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCameraComponent> Camera;

    //  Orbit configuration 

    /** Orbit radius from the center of the world (UE units, cm). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
    float OrbitRadius = 800.f;

    /** Angular speed in radians per second. 2*PI = one full orbit per second. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
    float AngularSpeed = 2.0f;

    /** Deadzone to avoid jitter. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float OrbitalDeadzone = 0.05f;

    //  Combat 

    /** Distance from ship origin at which projectiles spawn. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float ProjectileSpawnOffset = 30.f;

    /** Projectile class spawned when firing. Assigned in BP subclass. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TSubclassOf<class AProjectile> ProjectileClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 MaxHealth = 3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 CurrentHealth = 3;

    //  Input assets (assigned in Blueprint) 

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<class UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<class UInputAction> FireAction;

private:
    //  Runtime state 

    /** Current angle on the orbit (radians). */
    float CurrentAngle = 0.f;
    FVector2D InputVector = FVector2D::ZeroVector;

    //  Internal methods 

    /** Recomputes actor location and rotation from CurrentAngle + OrbitRadius. */
    void UpdateOrbitalPosition();

    /** Input handlers. */
    void Move(const struct FInputActionValue& Value);
    void StopMove(const struct FInputActionValue& Value);
    void Fire();
    
    UFUNCTION()
    void HandleOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};