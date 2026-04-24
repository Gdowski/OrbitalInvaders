// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameState.h"

#include "OrbitalPlayerController.h"
#include "OrbitalSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AOrbitalGameState::BeginPlay()
{
	Super::BeginPlay();
	LoadHighScore();
}

void AOrbitalGameState::SetGameplayState(EGameplayState NewState)
{
	GameplayState = NewState;
}

void AOrbitalGameState::AddScore(int32 Delta)
{
	Score += Delta;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d (+%d)"), Score, Delta);
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (AOrbitalPlayerController* OPC = Cast<AOrbitalPlayerController>(PC))
			{
				OPC->GetOrbitalHUD()->UpdateScore(Score);
			}
		}
	}
}

void AOrbitalGameState::AddScoreFor(EScoreEvent Event)
{
	int32 Value = 0;
	switch (Event)
	{
	case EScoreEvent::InvaderKilled:         Value = 100;  break;
	case EScoreEvent::SpecialInvaderKilled:  Value = 1000; break;
	case EScoreEvent::AsteroidSmallHit:      Value = 50;   break;
	case EScoreEvent::AsteroidMediumHit:     Value = 100;  break;
	case EScoreEvent::AsteroidLargeHit:      Value = 200;  break;
	}
	AddScore(Value);
}

void AOrbitalGameState::LoadHighScore()
{
	if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(UOrbitalSaveGame::SaveSlotName, 0))
	{
		if (UOrbitalSaveGame* Save = Cast<UOrbitalSaveGame>(Loaded))
		{
			HighScore = Save->HighScore;
			UE_LOG(LogTemp, Warning, TEXT("High score loaded: %d"), HighScore);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No save file found, starting fresh"));
	}
}

void AOrbitalGameState::SaveHighScoreIfNeeded()
{
	if (Score <= HighScore) return;

	HighScore = Score;

	UOrbitalSaveGame* Save = Cast<UOrbitalSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UOrbitalSaveGame::StaticClass()));

	if (Save)
	{
		Save->HighScore = HighScore;
		UGameplayStatics::SaveGameToSlot(Save, UOrbitalSaveGame::SaveSlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("New high score saved: %d"), HighScore);
	}
}