// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Invader.generated.h"

/**
 * AInvader - single invader in the formation.
 *
 * Does not tick - position is dictated by
 * SetOrbitalPosition. Fires projectiles on Fire().
 * Dies on one projectile hit or collision with a bunker.
 */
UCLASS()
class ORBITALINVADERS_API AInvader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvader();
	/** Index of the orbit this invader belongs to (0 = innermost) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Formation")
	int32 OrbitIndex = 0;

	/** Angular offset of this invader within the formation (radians) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Formation")
	float AngleOffset = 0.f;

	/** Set position on orbit. Called by formation manager. */
	UFUNCTION(BlueprintCallable, Category = "Orbit")
	void SetOrbitalPosition(float Angle, float OrbitRadius, float RotationDirection = 0.f);
	/** Apply damage. Returns true if the invader died as a result. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ApplyDamage(int32 Amount);
	
	/** Fire a projectile toward Earth. Called by formation manager. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Fire();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> InvaderMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<class USoundBase> ShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<class USoundBase> DeathSound;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 MaxHealth = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentHealth = 1;

	/** Projectile class to spawn when firing. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Offset from invader center at which projectile spawns. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ProjectileSpawnOffset = 30.f;

	/** Called when this invader dies (HP <= 0). Override for custom death behavior. */
	virtual void OnDeath();
	
	/** Called when killed by player projectile. Override for special effects. */
	virtual void OnPlayerKill();
	
	/** Niagara effect spawned on death. Assigned in BP. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> DeathExplosionEffect;
	
	/** Maximum roll angle when formation is rotating (degrees). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	float MaxRollAngle = 10.f;
private:
	/** Callback bound to overlap events. */
	UFUNCTION()
	void HandleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
