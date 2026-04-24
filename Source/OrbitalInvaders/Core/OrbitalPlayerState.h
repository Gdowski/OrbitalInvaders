// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OrbitalPlayerState.generated.h"

/**
 * Reserved: in a multiplayer context, per-player data (score, lives)
 * would live here instead of in AOrbitalGameState.
 */
UCLASS()
class ORBITALINVADERS_API AOrbitalPlayerState : public APlayerState
{
	GENERATED_BODY()

};
