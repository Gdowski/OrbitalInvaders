#include "VFXHelper.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UVFXHelper::SpawnExplosion(
	const UObject* WorldContextObject,
	UNiagaraSystem* Effect,
	const FVector& Location,
	float Scale)
{
	if (!Effect || !WorldContextObject) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		WorldContextObject,
		Effect,
		Location,
		FRotator::ZeroRotator,
		FVector(Scale),
		true,   // auto-destroy when complete
		true,   // auto-activate
		ENCPoolMethod::None
	);
}

void UVFXHelper::PlaySFX2D(const UObject* WorldContextObject, USoundBase* Sound)
{
	if (!Sound || !WorldContextObject) return;
	UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
}