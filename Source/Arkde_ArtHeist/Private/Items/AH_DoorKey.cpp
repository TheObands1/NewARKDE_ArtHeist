// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AH_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "AH_Character.h"

AAH_DoorKey::AAH_DoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "KeyA";
	XPValue = 40.0f;
}

void AAH_DoorKey::Pickup(AAH_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	PickupCharacter->AddKey(KeyTag);
	PickupCharacter->GainUltimateXP(XPValue);
	Destroy();
}
