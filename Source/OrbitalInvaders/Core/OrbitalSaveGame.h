#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitalSaveGame.generated.h"

/**
 * UOrbitalSaveGame - persists high score between sessions.
 */
UCLASS()
class ORBITALINVADERS_API UOrbitalSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 HighScore = 0;

	/** Slot name used for saving/loading. */
	static const FString SaveSlotName;
};