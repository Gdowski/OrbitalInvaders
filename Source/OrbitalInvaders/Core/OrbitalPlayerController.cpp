// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "OrbitalHUD.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitalInvaders/Entities/Earth.h"
#include "OrbitalInvaders/Entities/PlayerShip.h"

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
	
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UOrbitalHUD>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			if (AEarth* Earth = Cast<AEarth>(UGameplayStatics::GetActorOfClass(GetWorld(), AEarth::StaticClass())))
			{
				HUDWidget->InitEarthHealth(Earth->GetMaxHealth());
			}
			if (APlayerShip* PlayerShip = Cast<APlayerShip>(UGameplayStatics::GetPlayerPawn(this, 0)))
			{
				HUDWidget->InitPlayerHealth(PlayerShip->GetMaxHealth());
			}
			HUDWidget->AddToViewport(0);
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

AOrbitalPlayerController* AOrbitalPlayerController::Get(const UObject* WorldContextObject)
{
	return Cast<AOrbitalPlayerController>(
		UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

UOrbitalHUD* AOrbitalPlayerController::GetHUDFrom(const UObject* WorldContextObject)
{
	AOrbitalPlayerController* PC = Get(WorldContextObject);
	return PC ? PC->GetOrbitalHUD() : nullptr;
}