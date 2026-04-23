// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invader.h"
#include "SpecialInvader.generated.h"

/**
 * ASpecialInvader - rare enemy that spirals from the outer playfield
 * toward Earth, independently of the formation.
 *
 * Inherits from AInvader: same HP, damage handling, and Fire().
 * Overrides movement: uses an Archimedes spiral trajectory driven by Tick.
 */
UCLASS()
class ORBITALINVADERS_API ASpecialInvader : public AInvader
{
	GENERATED_BODY()

public:
	ASpecialInvader();

	virtual void Tick(float DeltaTime) override;
	/** Scale radial and angular speed. Called by spawner after spawn. */
	void SetSpeedMultiplier(float Multiplier);

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
	float StartRadius = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
	float RadialSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
	float AngularSpeed = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
	float DestroyRadius = 400.f;
	
	virtual EScoreEvent GetScoreEvent() const override { return EScoreEvent::SpecialInvaderKilled; }

private:
	/** Accumulated time since spawn, drives spiral motion. */
	float TimeAlive = 0.f;

	/** Initial angular offset (random per spawn). */
	float InitialAngle = 0.f;
};