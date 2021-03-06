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
#include "Components/WidgetComponent.h"
#include "UI/Enemy/AH_EnemyHealthBar.h"
#include "Core/AH_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAH_Enemy::AAH_Enemy()
{

	WidgetHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBarComponent"));
	WidgetHealthBarComponent->SetupAttachment(RootComponent);
	WidgetHealthBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CharacterType = EAH_CharacterType::CharacterType_Enemy;
	bLoopPath = false;
	bIsEnemyDefeated = false;
	DirectionIndex = 1;
	WaitingTimeOnPathPoint = 1;
	WeaponErrorMargin = 50;
	XPValue = 35.0f;
	LootProbability = 100.0f;
}

void AAH_Enemy::BeginPlay()
{
	Super::BeginPlay();

	Super::HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_Enemy::EnemyIsDamaged);

	Super::HealthComponent->OnDeadDelegate.AddDynamic(this, &AAH_Enemy::GiveXP);

	MyAIController = Cast<AAH_AIController>(GetController());

	if (IsValid(WidgetHealthBarComponent->GetUserWidgetObject()))
	{
		EnemyHealthBarReference = Cast<UAH_EnemyHealthBar>(WidgetHealthBarComponent->GetUserWidgetObject());
		if (IsValid(EnemyHealthBarReference))
		{
			Super::HealthComponent->OnHealthUpdateDelegate.AddDynamic(EnemyHealthBarReference, &UAH_EnemyHealthBar::UpdateHealth);
			HideHealthBar();
		}
	}

}

void AAH_Enemy::EnemyIsDamaged(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealthComponent->IsDead())
	{
		OnNotifyOfDeathDelegate.Broadcast(DamagedActor, Damage);
		SetIsAlert(false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeArmDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (IsValid(MyAIController))
		{
			MyAIController->DeactivateAIPerception();
			MyAIController->UnPossess();
			MyAIController->Destroy();
		}

		if (IsValid(GameInstanceReference) && !bIsEnemyDefeated)
		{
			bIsEnemyDefeated = true;
			GameInstanceReference->AddEnemyDefeatedToCounter();
		}

		if (IsValid(EnemyHealthBarReference))
		{
			HideHealthBar();
		}

	}
	else
	{
		OnCallForHealthDelegate.Broadcast(DamagedActor, Damage);

		if (bIsShowingHealthBar)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideHealthBar);
		}
		else
		{
			ShowHealthBar();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideHealthBar, this, &AAH_Enemy::HideHealthBar, 1.0f, false);
		}

		if (IsValid(MyAIController))
		{
			AAH_Rifle* PossibleRifle = Cast<AAH_Rifle>(DamageCauser);
			AAH_Character* PossiblePlayer = Cast<AAH_Character>(DamageCauser);
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

void AAH_Enemy::HideHealthBar()
{
	bIsShowingHealthBar = false;
	EnemyHealthBarReference->SetVisibility(ESlateVisibility::Hidden);
}

void AAH_Enemy::ShowHealthBar()
{
	bIsShowingHealthBar = true;
	EnemyHealthBarReference->SetVisibility(ESlateVisibility::Visible);
}

bool AAH_Enemy::TryToSpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	const float SelectedProbability = FMath::FRandRange(0, 100);

	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AAH_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameter);
		
		return true;
	}

	return false;
}

void AAH_Enemy::SetIsAlert(const bool NewValue)
{
	bIsAlert = NewValue;
	if (IsValid(GameModeReference))
	{
		GameModeReference->CheckAlertMode();
	}
}
