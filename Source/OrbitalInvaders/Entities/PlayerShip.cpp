// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"

#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "DrawDebugHelpers.h"

#include "Projectile.h"
#include "OrbitalInvaders/Core/OrbitalGameMode.h"
#include "OrbitalInvaders/Core/OrbitalPlayerController.h"
#include "OrbitalInvaders/Systems/VFXHelper.h"
// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Player"));
	RootComponent = CollisionComponent;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 3000.f;                     
	SpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); 
	SpringArm->bDoCollisionTest = false;                     
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;	
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CurrentAngle = PI / 2.f;
	//initialize the position of the player
	UpdateOrbitalPosition();
	
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::HandleOverlap);
	}
	
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAngle += InputDirection * AngularSpeed * DeltaTime;
	CurrentAngle = FMath::Fmod(CurrentAngle, 2.f * PI);
	
	const float TargetRoll = InputDirection * MaxRollAngle;
	CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, DeltaTime, RollInterpSpeed);

	UpdateOrbitalPosition();
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShip::Move);
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerShip::StopMove);
		}
		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerShip::Fire);
		}
		if (PauseAction)
		{
			EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerShip::TogglePause);
		}
	}
	
}

void APlayerShip::UpdateOrbitalPosition()
{
	// 1. Calculate Pos
	const float X = FMath::Cos(CurrentAngle) * OrbitRadius;
	const float Y = FMath::Sin(CurrentAngle) * OrbitRadius;
	const FVector NewLocation(X, Y, 0.f);

	SetActorLocation(NewLocation);

	// 2. Calculate rotation -> always away from center
	const FVector DirectionToCenter = (NewLocation).GetSafeNormal();
	FRotator NewRotation = DirectionToCenter.Rotation();
	NewRotation.Roll = CurrentRoll;
	SetActorRotation(NewRotation);
	
}

void APlayerShip::Move(const FInputActionValue& Value)
{
	InputDirection = Value.Get<float>();

}

void APlayerShip::StopMove(const FInputActionValue& Value)
{
	InputDirection = 0.0f;
}

void APlayerShip::Fire()
{
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;
	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastFireTime <= FireCooldown)
	{
		return;
	}
	LastFireTime = Now;
	if (ShootSound)
	{
		UVFXHelper::PlaySFX2D(this, ShootSound);
	}
	// Fire direction = outwards Earth 
	const FVector FireDirection = GetActorLocation().GetSafeNormal();
	const FVector SpawnLocation = GetActorLocation() + FireDirection*ProjectileSpawnOffset;

	// Spawn the projectile facing the direction of travel
	const FRotator SpawnRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Init(FireDirection);
	}
}

int32 APlayerShip::ApplyDamage(int32 Amount)
{
	CurrentHealth = FMath::Max(0, CurrentHealth - Amount);
	if (HitSound)
	{
		UVFXHelper::PlaySFX2D(this, HitSound);
	}
	// Screen shake on hit + HUD update
	if (AOrbitalPlayerController* PC = Cast<AOrbitalPlayerController>(GetController()))
	{
		PC->PlayCameraShake(1.f);  // full intensity for player hit
		if (UOrbitalHUD* HUD = PC->GetOrbitalHUD())
		{
			HUD->UpdatePlayerHealth(CurrentHealth);
		}
	}
	
	if (CurrentHealth <= 0)
	{
		if (AOrbitalGameMode* GM = GetWorld()->GetAuthGameMode<AOrbitalGameMode>())
		{
			GM->TriggerGameOver(TEXT("Player destroyed"));
		}
		
	}
	else
	{
		bIsInvincible = true;

		// End invincibility after duration
		GetWorld()->GetTimerManager().SetTimer(
			InvincibilityTimerHandle,
			this,
			&APlayerShip::EndInvincibility,
			InvincibilityDuration,
			false
		);

		// Blink mesh during invincibility
		GetWorld()->GetTimerManager().SetTimer(
			BlinkTimerHandle,
			this,
			&APlayerShip::ToggleMeshVisibility,
			0.1f,
			true  // looping
		);
	}
	return CurrentHealth;
}

void APlayerShip::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	if (bIsInvincible) return;

	// Hit by projectile
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		ApplyDamage(1);
		Projectile->Destroy();
		return;
	}

	// Hit by asteroid
	if (Cast<AAsteroid>(OtherActor))
	{
		ApplyDamage(1);
		return;
	}
}

void APlayerShip::EndInvincibility()
{
	bIsInvincible = false;

	// Stop blinking, ensure mesh is visible
	GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
	if (ShipMesh)
	{
		ShipMesh->SetVisibility(true);
	}
}

void APlayerShip::ToggleMeshVisibility()
{
	if (ShipMesh)
	{
		ShipMesh->ToggleVisibility();
	}
}

void APlayerShip::TogglePause()
{
	if (AOrbitalPlayerController* PC = Cast<AOrbitalPlayerController>(GetController()))
	{
		if (PC->IsPaused())
		{
			PC->SetPause(false);
			if (PauseWidget) PauseWidget->RemoveFromParent();
		}
		else
		{
			PC->SetPause(true);
			if (PauseWidgetClass)
			{
				PauseWidget = CreateWidget<UUserWidget>(PC, PauseWidgetClass);
				if (PauseWidget)
				{
					PauseWidget->AddToViewport(1);
				}
			}
		}
	}
}
