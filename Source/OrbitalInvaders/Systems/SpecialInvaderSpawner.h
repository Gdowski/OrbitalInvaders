#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialInvaderSpawner.generated.h"

/**
 * ASpecialInvaderSpawner - periodically spawns a special invader that
 * spirals toward Earth independently of the main formation.
 */
UCLASS()
class ORBITALINVADERS_API ASpecialInvaderSpawner : public AActor
{
	GENERATED_BODY()

public:
	ASpecialInvaderSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<class ASpecialInvader> SpecialInvaderClass;

	/** Time between spawns (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 15.f;

	/** Delay before first spawn after level start (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float InitialDelay = 10.f;

private:
	float TimeSinceLastSpawn = 0.f;
	bool bFirstSpawnDone = false;

	void SpawnSpecialInvader();
};