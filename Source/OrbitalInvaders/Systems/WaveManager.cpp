#include "WaveManager.h"
#include "InvaderFormationManager.h"
#include "AsteroidSpawner.h"
#include "BunkerSpawner.h"
#include "SpecialInvaderSpawner.h"
#include "OrbitalInvaders/Core/OrbitalPlayerController.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    if (FormationManager)
    {
        FormationManager->OnFormationCleared.AddDynamic(this, &AWaveManager::OnFormationCleared);
    }
}

void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bWaitingForNextWave)
    {
        WaveDelayTimer += DeltaTime;
        if (WaveDelayTimer >= WaveDelay)
        {
            StartNextWave();
        }
    }
}



void AWaveManager::CleanupPlayfield()
{
    if (AsteroidSpawner) AsteroidSpawner->ClearAllAsteroids();
    if (SpecialInvaderSpawner) SpecialInvaderSpawner->ClearAndReset();
    // Bunkers stay damaged until respawn in StartNextWave
}

void AWaveManager::StartNextWave()
{
    CurrentWave++;
    bWaitingForNextWave = false;

    UE_LOG(LogTemp, Warning, TEXT("=== WAVE %d START ==="), CurrentWave);
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (AOrbitalPlayerController* OPC = Cast<AOrbitalPlayerController>(PC))
            {
                OPC->GetHUD()->UpdateWave(CurrentWave);
            }
        }
    }
    // Respawn bunkers fresh
    if (BunkerSpawner) BunkerSpawner->RespawnBunkers();

    // Apply scaling then spawn formation
    ApplyWaveScaling();

    if (FormationManager)
    {
        FormationManager->ResetFormation();
        FormationManager->SpawnFormation();
    }
}

void AWaveManager::ApplyWaveScaling()
{
    if (FormationManager)
    {
        FormationManager->SetRotationSpeed(FormationManager->GetRotationSpeed() * RotationSpeedScale);
        FormationManager->SetFireInterval(FormationManager->GetFireInterval() * FireIntervalScale);
        FormationManager->SetJumpInterval(FormationManager->GetJumpInterval() * JumpIntervalScale);
        FormationManager->SetInvadersPerOrbit(FormationManager->GetInvadersPerOrbit()+1);
        if (CurrentWave % 3 == 0)
        {
            FormationManager->SetOrbitCount(FormationManager->GetOrbitCount() + 1);
        }
        
    }
    if (AsteroidSpawner)
    {
        AsteroidSpawner->SetSpawnInterval(AsteroidSpawner->GetSpawnInterval() * AsteroidSpawnScale);
    }
    if (SpecialInvaderSpawner)
    {
        SpecialInvaderSpawner->SetSpeedMultiplier(
        SpecialInvaderSpawner->GetSpeedMultiplier() * SpecialInvaderSpeedScale);
    }
}

void AWaveManager::OnFormationCleared()
{
    if (bWaitingForNextWave) return;  // already triggered

    bWaitingForNextWave = true;
    WaveDelayTimer = 0.f;

    CleanupPlayfield();

    UE_LOG(LogTemp, Warning, TEXT("Wave %d cleared! Next wave in %.1fs"), CurrentWave, WaveDelay);
}

void AWaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (FormationManager)
    {
        FormationManager->OnFormationCleared.RemoveDynamic(this, &AWaveManager::OnFormationCleared);
    }

    Super::EndPlay(EndPlayReason);
}