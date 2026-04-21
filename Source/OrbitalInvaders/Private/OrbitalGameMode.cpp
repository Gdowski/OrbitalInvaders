// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameMode.h"
#include "OrbitalGameState.h"
#include "OrbitalPlayerController.h"
#include "OrbitalPlayerState.h"
#include "PlayerShip.h"

AOrbitalGameMode::AOrbitalGameMode()
{
	GameStateClass        = AOrbitalGameState::StaticClass();
	PlayerControllerClass = AOrbitalPlayerController::StaticClass();
	PlayerStateClass      = AOrbitalPlayerState::StaticClass();

	DefaultPawnClass = APlayerShip::StaticClass();
}

void AOrbitalGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY"));
}
