// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialInvader.h"

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