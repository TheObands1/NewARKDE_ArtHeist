// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AH_Weapon.h"
#include "GameFramework/Character.h"

// Sets default values
AAH_Weapon::AAH_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;

}

// Called when the game starts or when spawned
void AAH_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_Weapon::StartAction()
{
	BP_StartAction();
}

void AAH_Weapon::StopAction()
{
	BP_StopAction();
}

void AAH_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}


}

