// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalGameMode.generated.h"

/**
 * AOrbitalGameMode - top-level game rules controller.
 *
 * Responsibilities:
 *   - Wires up default classes (GameState, PlayerController, PlayerState, Pawn)
 *   - Handles Game Over transition via TriggerGameOver
 *
 * BP subclass (BP_OrbitalGameMode) is used in DefaultEngine.ini,
 * with all default classes explicitly assigned in BP Class Defaults.
 */
UCLASS()
class ORBITALINVADERS_API AOrbitalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AOrbitalGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TriggerGameOver(const FString& Reason);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<class USoundBase> BackgroundMusic;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<class USoundBase> GameOverSound;
	
protected:
	virtual void BeginPlay() override;
};
