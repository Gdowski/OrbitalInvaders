// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrbitalHUD.generated.h"

/**
 * 
 */
UCLASS()
class ORBITALINVADERS_API UOrbitalHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWave(int32 NewWave);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerHealth(int32 NewHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEarthHealth(int32 NewHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 NewScore);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitPlayerHealth(int32 MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void InitEarthHealth(int32 MaxHealth);
};
