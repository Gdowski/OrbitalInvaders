// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderProjectile.h"

#include "Components/SphereComponent.h"

AInvaderProjectile::AInvaderProjectile()
{
	// Override collision profile from parent class
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionProfileName(TEXT("InvaderProjectile"));
	}
}
