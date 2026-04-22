// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalPlayerController.h"

#include "EnhancedInputSubsystems.h"

void AOrbitalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AOrbitalPlayerController::PlayCameraShake(float Intensity)
{
	if (HitShakeClass)
	{
		ClientStartCameraShake(HitShakeClass, Intensity);
	}
}