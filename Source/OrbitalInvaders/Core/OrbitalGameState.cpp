// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameState.h"

void AOrbitalGameState::SetGameplayState(EGameplayState NewState)
{
	GameplayState = NewState;
}

void AOrbitalGameState::AddScore(int32 Delta)
{
	Score += Delta;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d (+%d)"), Score, Delta);
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