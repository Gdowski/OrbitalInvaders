// Fill out your copyright notice in the Description page of Project Settings.

#include "InvaderFormationManager.h"
#include "OrbitalInvaders/Core/OrbitalGameMode.h"
#include "OrbitalInvaders/Entities/Invader.h"





AInvaderFormationManager::AInvaderFormationManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AInvaderFormationManager::BeginPlay()
{
    Super::BeginPlay();
    SpawnFormation();
}

void AInvaderFormationManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Reverse rotation direction periodically
    TimeSinceLastReversal += DeltaTime;
    if (TimeSinceLastReversal >= RotationReversalInterval)
    {
        TimeSinceLastReversal = 0.f;
        RotationDirection *= -1.f;
    }

    FormationAngle += RotationSpeed * RotationDirection * DeltaTime;
    FormationAngle = FMath::Fmod(FormationAngle, 2.f * PI);

    // Jump timer
    TimeSinceLastJump += DeltaTime;
    if (TimeSinceLastJump >= JumpInterval)
    {
        TimeSinceLastJump = 0.f;
        PerformJump();
    }

    UpdateInvaderPositions();
    TimeSinceLastFire += DeltaTime;
    if (TimeSinceLastFire >= FireInterval)
    {
        TimeSinceLastFire = 0.f;
        FireRandomInvader();
    }
    
    // Check game over condition: innermost orbit reached player
    if (Invaders.Num() > 0 && GetOrbitRadius(0) <= GameOverRadius)
    {
        if (AOrbitalGameMode* GM = GetWorld()->GetAuthGameMode<AOrbitalGameMode>())
        {
            GM->TriggerGameOver(TEXT("Invaders breached player orbit"));
        }
    }
}

void AInvaderFormationManager::SpawnFormation()
{
    if (!InvaderClass)
    {
        UE_LOG(LogTemp, Error, TEXT("InvaderFormationManager: InvaderClass is not set"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    const float AngleStep = 2.f * PI / static_cast<float>(InvadersPerOrbit);

    for (int32 OrbitIdx = 0; OrbitIdx < OrbitCount; ++OrbitIdx)
    {
        for (int32 InvaderIdx = 0; InvaderIdx < InvadersPerOrbit; ++InvaderIdx)
        {
            const float Offset = AngleStep * static_cast<float>(InvaderIdx) + 0.1f * static_cast<float>(OrbitIdx);

            // Spawn at origin; position will be set in SetOrbitalPosition
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;    

            AInvader* NewInvader = World->SpawnActor<AInvader>(
                InvaderClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

            if (NewInvader)
            {
                NewInvader->OrbitIndex = OrbitIdx;
                NewInvader->AngleOffset = Offset;
                Invaders.Add(NewInvader);
            }
        }
    }

    UpdateInvaderPositions();
}

void AInvaderFormationManager::PerformJump()
{
    FormationRadialOffset += JumpDistance;
}

void AInvaderFormationManager::UpdateInvaderPositions()
{
    // Clean up destroyed invaders from the array
    Invaders.RemoveAll([](const TObjectPtr<AInvader>& Inv)
    {
        return !IsValid(Inv);
    });
    
    // Emit event when formation is cleared
    if (Invaders.Num() == 0 && !bFormationClearedBroadcasted)
    {
        bFormationClearedBroadcasted = true;
        OnFormationCleared.Broadcast();
        return;
    }

    for (AInvader* Invader : Invaders)
    {
        if (!Invader) continue;

        const float Angle = FormationAngle + Invader->AngleOffset;
        const float Radius = GetOrbitRadius(Invader->OrbitIndex);
        Invader->SetOrbitalPosition(Angle, Radius,RotationDirection);
    }
}

float AInvaderFormationManager::GetOrbitRadius(int32 OrbitIndex) const
{
    const float BaseRadius = InnermostRadius + static_cast<float>(OrbitIndex) * OrbitSpacing;
    return BaseRadius - FormationRadialOffset;
}

void AInvaderFormationManager::FireRandomInvader()
{
    if (Invaders.Num() == 0) return;

    const int32 RandIndex = FMath::RandRange(0, Invaders.Num() - 1);
    if (AInvader* Shooter = Invaders[RandIndex])
    {
        Shooter->Fire();
    }
}

void AInvaderFormationManager::ClearFormation()
{
    for (AInvader* Invader : Invaders)
    {
        if (IsValid(Invader))
        {
            Invader->Destroy();
        }
    }
    Invaders.Empty();
}

void AInvaderFormationManager::ResetFormation()
{
    FormationAngle = 0.f;
    FormationRadialOffset = 0.f;
    TimeSinceLastJump = 0.f;
    TimeSinceLastFire = 0.f;
    TimeSinceLastReversal = 0.f;
    RotationDirection = 1.f;
    bFormationClearedBroadcasted = false;
}