#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BunkerSpawner.generated.h"

/**
 * ABunkerSpawner - spawns bunkers distributed evenly on a circular orbit
 * between the player and the invaders.
 */
UCLASS()
class ORBITALINVADERS_API ABunkerSpawner : public AActor
{
	GENERATED_BODY()

public:
	ABunkerSpawner();
	
	void RespawnBunkers();

protected:
	virtual void BeginPlay() override;

	/** Bunker class to spawn. Assigned in BP subclass. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bunker")
	TSubclassOf<class ABunker> BunkerClass;

	/** Number of bunkers to spawn around Earth. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bunker", meta = (ClampMin = "1"))
	int32 BunkerCount = 6;

	/** Radius at which bunkers are placed (UE units). Should be between player orbit and invader orbit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bunker")
	float BunkerOrbitRadius = 1100.f;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<class ABunker>> SpawnedBunkers;
	
	void SpawnBunkers();
};