// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AH_GrenadeLauncher.h"
#include "Weapons/AH_Projectile.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


AAH_GrenadeLauncher::AAH_GrenadeLauncher()
{
	MuzzleSocketName = "SCK_Muzzle";
}



void AAH_GrenadeLauncher::StartAction()
{
	Super::StartAction(); //Call Father to maintain structure

	if(IsValid(CurrentOwnerCharacter))
	{
		USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();

		if (IsValid(CharacterMeshComponent))
		{
			FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
			FRotator  MuzzleSocketRotator = CharacterMeshComponent->GetSocketRotation(MuzzleSocketName);

			AAH_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AAH_Projectile>(ProjectileClass, MuzzleSocketLocation, MuzzleSocketRotator);
			CurrentProjectile->SetOwner(CurrentOwnerCharacter);
		}

	}
	


}

void AAH_GrenadeLauncher::StopAction()
{

	Super::StopAction(); //Call Father to maintain structure

}

