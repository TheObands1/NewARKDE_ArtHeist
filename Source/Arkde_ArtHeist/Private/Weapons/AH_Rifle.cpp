// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AH_Rifle.h"
#include "Arkde_ArtHeist/Arkde_ArtHeist.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AH_Character.h"
#include "Enemy/AH_Enemy.h"


AAH_Rifle::AAH_Rifle()
{
	TraceLenght = 10000.0f;
	MuzzleSocketName = "SCK_Muzzle";
}

void AAH_Rifle::StartAction()
{
	Super::StartAction(); //Call Father to maintain structure

	AActor* CurrentOwner = GetOwner();
	//Valids if character is the owner of the weapon
	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation; //Start of the LineTrace
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;
		
		
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);  

		//If Linetrace hits something
		if (bHit)
		{
			//Make Damage
			AActor* HitActor = HitResult.GetActor();

			if (IsValid(HitActor))
			{
				AAH_Enemy* EnemyOwner = Cast<AAH_Enemy>(CurrentOwner);
				AAH_Character* HitCharacter = Cast<AAH_Character>(HitActor);
				if (IsValid(HitCharacter))
				{
					BP_CheckHitResult(HitResult);
					bool bIsHitCharacterAPlayer = (HitCharacter->GetCharacterType() == EAH_CharacterType::CharacterType_Player);
					//If the linetrace hits a player and the owner is a enemy
					if (bIsHitCharacterAPlayer && IsValid(EnemyOwner))
					{
						int randomnumber = FMath::RandRange(0, 100);
						if (randomnumber >= EnemyOwner->WeaponErrorMargin)
						{
							UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
						}
						else
						{
							UGameplayStatics::ApplyPointDamage(HitActor, 0, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
						}
					}
					else
					{
						UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
					}
				}
				else
				{
					UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
				}
			}

			TraceEndPoint = HitResult.ImpactPoint;

			if (IsValid(ImpactEffect))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
			}
			
		}
		if (bDrawLineTrace)
		{
			UE_LOG(LogTemp, Log, TEXT("Entering Correctly"));
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0.0f, 1.0f);
		}

		if (IsValid(MuzzleEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);
		}


		if (IsValid(TraceEffect))
		{
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();

			if (IsValid(CharacterMeshComponent))
			{
				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleSocketLocation);

				if (IsValid(TraceComponent))
				{
					TraceComponent->SetVectorParameter(TraceParamName, TraceEndPoint);
				}
			}
		}

	}

}

void AAH_Rifle::StopAction()
{
	Super::StopAction(); //Call Father to maintain structure
}
