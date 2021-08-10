// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AH_HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "AH_Character.h"

AAH_HealthBox::AAH_HealthBox()
{
	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;
}

void AAH_HealthBox::Pickup(AAH_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	const bool bIsHealingSuccessfull = PickupCharacter->TryToAddHealth(HealthValue);

	if (bIsHealingSuccessfull)
	{
		PlayPickUpSound();
		Destroy();
	}

}
