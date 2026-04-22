// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalGameState.h"

void AOrbitalGameState::SetGameplayState(EGameplayState NewState)
{
	GameplayState = NewState;
}

void AOrbitalGameState::AddScore(int32 Delta)
{
	Score += Delta;
}