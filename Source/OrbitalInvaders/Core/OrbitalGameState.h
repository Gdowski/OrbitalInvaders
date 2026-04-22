// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbitalGameState.generated.h"

/** High-level gameplay state */
UENUM(BlueprintType)
enum class EGameplayState : uint8
{
	Playing     UMETA(DisplayName = "Playing"),
	GameOver    UMETA(DisplayName = "Game Over"),
};
/**
 * 
 */
UCLASS()
class ORBITALINVADERS_API AOrbitalGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	EGameplayState GetGameplayState() const { return GameplayState; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetGameplayState(EGameplayState NewState);

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Delta);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	EGameplayState GameplayState = EGameplayState::Playing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;
};
