#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VFXHelper.generated.h"

/**
 * UVFXHelper - static utility functions for spawning visual effects.
 */
UCLASS()
class ORBITALINVADERS_API UVFXHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Spawn a Niagara effect at the given world location. */
	UFUNCTION(BlueprintCallable, Category = "VFX", meta = (WorldContext = "WorldContextObject"))
	static void SpawnExplosion(
		const UObject* WorldContextObject,
		class UNiagaraSystem* Effect,
		const FVector& Location,
		float Scale = 1.f);
};