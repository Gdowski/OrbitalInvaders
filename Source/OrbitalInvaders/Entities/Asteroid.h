#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

UENUM(BlueprintType)
enum class EAsteroidSize : uint8
{
	Large   UMETA(DisplayName = "Large"),
	Medium  UMETA(DisplayName = "Medium"),
	Small   UMETA(DisplayName = "Small"),
};

/**
 * AAsteroid - linear-moving obstacle that wraps around the playfield.
 *
 * Spawned by AAsteroidSpawner which also manages wrap-around.
 * On hit by projectile or collision with other entities, splits
 */
UCLASS()
class ORBITALINVADERS_API AAsteroid : public AActor
{
	GENERATED_BODY()

public:
	AAsteroid();
	
	UFUNCTION(BlueprintCallable, Category = "Asteroid")
	void Init(const FVector& InVelocity, EAsteroidSize InSize);

	UFUNCTION(BlueprintCallable, Category = "Asteroid")
	void SplitOrDestroy();

protected:
	virtual void BeginPlay() override;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> AsteroidMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

	// Data 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid")
	EAsteroidSize Size = EAsteroidSize::Large;
	
private:
	UFUNCTION()
	void HandleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	bool bIsBeingDestroyed = false;
};