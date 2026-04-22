// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameMode.h"
#include "OrbitalGameState.h"
#include "OrbitalPlayerController.h"
#include "OrbitalPlayerState.h"
#include "OrbitalInvaders/Entities/PlayerShip.h"

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

void AOrbitalGameMode::TriggerGameOver(const FString& Reason)
{
	AOrbitalGameState* GS = GetGameState<AOrbitalGameState>();
	if (!GS) return;

	if (GS->GetGameplayState() == EGameplayState::GameOver)
	{
		return;
	}

	GS->SetGameplayState(EGameplayState::GameOver);
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER Reason: %s"), *Reason);

	UWorld* World = GetWorld();
	if (!World) return;

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		bool bResult = PC->SetPause(true);
	}
}