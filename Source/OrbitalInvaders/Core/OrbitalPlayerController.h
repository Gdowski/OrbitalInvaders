// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbitalHUD.h"
#include "GameFramework/PlayerController.h"
#include "OrbitalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ORBITALINVADERS_API AOrbitalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
public:
	/** Trigger camera shake with given intensity (0.0 - 1.0). */
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayCameraShake(float Intensity = 1.f);

	UOrbitalHUD* GetOrbitalHUD() const { return HUDWidget; }

	/** Safely retrieves the OrbitalPlayerController from any world context. Returns nullptr if unavailable. */
	static AOrbitalPlayerController* Get(const UObject* WorldContextObject);

	/** Safely retrieves the HUD widget via PlayerController. Returns nullptr if PC or HUD is unavailable. */
	static UOrbitalHUD* GetHUDFrom(const UObject* WorldContextObject);

protected:
	/** Camera shake class used for gameplay impacts. Assigned in BP. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> HitShakeClass;
	
	/** HUD widget class. Assigned in BP. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UOrbitalHUD> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UOrbitalHUD> HUDWidget;
	
	
};
