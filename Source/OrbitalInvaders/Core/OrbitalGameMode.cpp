// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameMode.h"
#include "OrbitalGameState.h"
#include "OrbitalPlayerController.h"
#include "OrbitalPlayerState.h"
#include "Kismet/GameplayStatics.h"
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
	if (BackgroundMusic)
	{
		UGameplayStatics::PlaySound2D(this, BackgroundMusic, 0.5f, 1.f, 0.f);
	}
}

void AOrbitalGameMode::TriggerGameOver(const FString& Reason)
{
	AOrbitalGameState* GS = GetGameState<AOrbitalGameState>();
	if (!GS) return;

	if (GS->GetGameplayState() == EGameplayState::GameOver)
	{
		return;
	}
	if (GameOverSound)
	{
		UGameplayStatics::PlaySound2D(this, GameOverSound, 1.f);
	}
	GS->SetGameplayState(EGameplayState::GameOver);
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER Reason: %s"), *Reason);
	GS->SaveHighScoreIfNeeded();
	
	UWorld* World = GetWorld();
	if (!World) return;

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		bool bResult = PC->SetPause(true);
	}
	
	//here need to add the event dispatch	
	OnGameLost.Broadcast(Reason);
}