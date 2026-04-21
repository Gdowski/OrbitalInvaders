// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "DrawDebugHelpers.h"

#include "Projectile.h"
// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CollisionComponent;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 3000.f;                     
	SpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); 
	SpringArm->bDoCollisionTest = false;                     
	SpringArm->bInheritPitch = false;                        
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bUsePawnControlRotation = false;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	CurrentAngle = PI / 2.f;
	//initialize the position of the player
	UpdateOrbitalPosition();
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Tangent vector: perpendicular to radius, pointing CCW
	const FVector2D Tangent(-FMath::Sin(CurrentAngle), FMath::Cos(CurrentAngle));

	// Dot product: how much player input aligns with orbit direction
	const float OrbitalDir = FVector2D::DotProduct(InputVector, Tangent);
	
	const float Direction = (FMath::Abs(OrbitalDir) > OrbitalDeadzone)
	? FMath::Sign(OrbitalDir)
	: 0.f;
	CurrentAngle += Direction * AngularSpeed * DeltaTime;
	CurrentAngle = FMath::Fmod(CurrentAngle, 2.f * PI);

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
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerShip::Fire);
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
	const FVector DirectionToCenter = (-NewLocation).GetSafeNormal();
	const FRotator NewRotation = DirectionToCenter.Rotation();

	SetActorRotation(NewRotation);
}

void APlayerShip::Move(const FInputActionValue& Value)
{
	InputVector = Value.Get<FVector2D>();

}

void APlayerShip::StopMove(const FInputActionValue& Value)
{
	InputVector = FVector2D::ZeroVector;
}

void APlayerShip::Fire()
{
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Fire direction = outwards Earth 
	const FVector FireDirection = GetActorLocation().GetSafeNormal();
	const FVector SpawnLocation = GetActorLocation() + FireDirection*ProjectileSpawnOffset;

	// Spawn the projectile facing the direction of travel
	const FRotator SpawnRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Init(FireDirection);
	}
}

