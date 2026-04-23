// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialInvader.h"

#include "Kismet/GameplayStatics.h"
#include "OrbitalInvaders/Systems/VFXHelper.h"

ASpecialInvader::ASpecialInvader()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpecialInvader::BeginPlay()
{
	Super::BeginPlay();

	// Random starting angle on the spiral
	InitialAngle = FMath::RandRange(0.f, 2.f * PI);
	TimeAlive = 0.f;
}

void ASpecialInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAlive += DeltaTime;

	// Archimedes spiral: r shrinks linearly, angle grows linearly
	const float Radius = StartRadius - RadialSpeed * TimeAlive;
	const float Angle = InitialAngle + AngularSpeed * TimeAlive;

	// Self-destruct when reaching inner zone (approaching Earth)
	if (Radius <= DestroyRadius)
	{
		Destroy();
		return;
	}

	const FVector NewLocation(
		FMath::Cos(Angle) * Radius,
		FMath::Sin(Angle) * Radius,
		0.f
	);
	SetActorLocation(NewLocation);

	const FVector Direction = (-NewLocation).GetSafeNormal();
	SetActorRotation(Direction.Rotation());
}
void ASpecialInvader::SetSpeedMultiplier(float Multiplier)
{
	RadialSpeed *= Multiplier;
	AngularSpeed *= Multiplier;
}

void ASpecialInvader::OnDeath()
{
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 0.15f);

		// Use a latent delay in real time
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, [World]()
		{
			UGameplayStatics::SetGlobalTimeDilation(World, 1.f);
		}, 0.4f, false);
		// 0.05 game-seconds at 0.15 dilation = ~0.33 real seconds of slow-mo
	}

	if (DeathExplosionEffect)
	{
		UVFXHelper::SpawnExplosion(this, DeathExplosionEffect, GetActorLocation(), 25.f);
	}

	if (AOrbitalGameState* GS = GetWorld()->GetGameState<AOrbitalGameState>())
	{
		GS->AddScoreFor(EScoreEvent::SpecialInvaderKilled);
	}

	if (DeathSound)
	{
		UVFXHelper::PlaySFX2D(this, DeathSound);
	}

	Destroy();
}