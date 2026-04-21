// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ORBITALINVADERS_API AOrbitalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AOrbitalGameMode();
	
protected:
	virtual void BeginPlay() override;
};
