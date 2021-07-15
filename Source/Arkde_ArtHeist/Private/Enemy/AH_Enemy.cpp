// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_Enemy.h"
#include "Weapons/AH_Weapon.h"
#include "Components/AH_HealthComponent.h"
#include "AH_Character.h"
#include "Weapons/AH_Rifle.h"
#include "Items/AH_Item.h"
#include "Enemy/Controller/AH_AIController.h"
#include "Components/ActorComponent.h"
#include "Perception/AISense_Damage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Core/AH_GameInstance.h"

// Sets default values
AAH_Enemy::AAH_Enemy()
{
	bLoopPath = false;
	bIsEnemyAfraid = false;
	bIsEnemyDefeated = false;
	DirectionIndex = 1;
	WaitingTimeOnPathPoint = 1;
	WeaponErrorMargin = 50;
	XPValue = 20.0f;
	LootProbability = 100.0f;
}

void AAH_Enemy::BeginPlay()
{
	Super::BeginPlay();

	Super::HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_Enemy::EnemyIsDamaged);

	Super::HealthComponent->OnDeadDelegate.AddDynamic(this, &AAH_Enemy::GiveXP);

	MyAIController = Cast<AAH_AIController>(GetController());

}

void AAH_Enemy::EnemyIsDamaged(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealthComponent->IsDead())
	{
		OnNotifyOfDeathDelegate.Broadcast(DamagedActor, Damage);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeArmDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (IsValid(MyAIController))
		{
			MyAIController->UnPossess();
		}

		if (IsValid(GameInstanceReference) && !bIsEnemyDefeated)
		{
			bIsEnemyDefeated = true;
			GameInstanceReference->AddEnemyDefeatedToCounter();
		}

	}
	else
	{
		OnCallForHealthDelegate.Broadcast(DamagedActor, Damage);

		if (IsValid(MyAIController))
		{
			AAH_Rifle* PossibleRifle = Cast<AAH_Rifle>(DamageCauser);
			if (IsValid(PossibleRifle))
			{
				MyAIController->SetIsReceivingDamage(true);
				AActor* RifleOwner = Cast<AActor>(PossibleRifle->GetOwner());
				UAISense_Damage::ReportDamageEvent(GetWorld(), this, RifleOwner, Damage, RifleOwner->GetActorLocation(), FVector::ZeroVector);
			}
		}
	}
}

void AAH_Enemy::GiveXP(AActor* DamageCauser)
{
	if (bIsEnemyDefeated)
	{
		return;
	}
	
	AAH_Character* PossiblePlayer = Cast<AAH_Character>(DamageCauser);

	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
		TryToSpawnLoot();
	}

	AAH_Rifle* PossibleRifle = Cast<AAH_Rifle>(DamageCauser);
	if (IsValid(PossibleRifle))
	{
		AAH_Character* PossibleRifleOwnerPlayer = Cast<AAH_Character>(PossibleRifle->GetOwner());
		if (IsValid(PossibleRifleOwnerPlayer) && PossibleRifleOwnerPlayer->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			PossibleRifleOwnerPlayer->GainUltimateXP(XPValue);
			TryToSpawnLoot();
		}
	}
}

bool AAH_Enemy::TryToSpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	float SelectedProbability = FMath::FRandRange(0, 100);

	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AAH_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameter);
		
		return true;
	}

	return false;
}
