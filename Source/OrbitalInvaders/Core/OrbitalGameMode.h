// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalGameMode.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLost,FString,Reason);


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
    // Constructor
    AOrbitalGameMode();

    // Public API
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void TriggerGameOver(const FString& Reason);

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Gameplay")
    FOnGameLost OnGameLost;

protected:
    // Virtual overrides
    virtual void BeginPlay() override;

    // SFX / VFX
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<class USoundBase> BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<class USoundBase> GameOverSound;
};
