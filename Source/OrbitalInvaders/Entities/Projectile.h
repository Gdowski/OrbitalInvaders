// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ORBITALINVADERS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	/** Fires the projectile in the given world-space direction. */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Init(const FVector& Direction);

	/** Returns true if this is a player-fired projectile (awards score on kill). */
	UFUNCTION(BlueprintPure, Category = "Projectile")
	virtual bool IsPlayerProjectile() const { return true; }

protected:
	virtual void BeginPlay() override;

	// Components

	/** Collision root. Protected so subclasses can override the collision profile. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> CollisionComponent;

	// Config

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan = 3.f;

private:
	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;
};
