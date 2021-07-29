// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_Bot.h"
#include "Enemy/AH_BotSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AH_Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Components/AH_HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystem.h"
#include "Weapons/AH_Rifle.h"
#include "Items/AH_Item.h"
#include "Core/AH_GameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AAH_Bot::AAH_Bot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetectorComponent"));
	SelfDestructionDetectorComponent->SetupAttachment(RootComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150);

	HealthComponent = CreateDefaultSubobject<UAH_HealthComponent>(TEXT("HealthComponent"));

	TimerSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TimerSoundComponent"));
	TimerSoundComponent->SetupAttachment(RootComponent);

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 1000.0f;
	ExplosionDamage = 50.0f;
	ExplosionRadius = 250.0f;
	XPValue = 20.0f;
	LootProbability = 100.0f;

}

// Called when the game starts or when spawned
void AAH_Bot::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		PlayerCharacter = Cast<AAH_Character>(PlayerPawn);
	}

	NextPathPoint = GetNextPathPoint();
	GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_Bot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AAH_Bot::GiveXP);
	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_Bot::StartCountDown);
	BotMaterial= BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));
	
}

void AAH_Bot::TakingDamage(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		if (IsValid(DamageCauser))
		{
			AAH_Character* PossiblePlayer = Cast<AAH_Character>(DamageCauser);
			if (IsValid(PossiblePlayer))
			{
				TryToSpawnLoot();
			}

			AAH_Rifle* PossibleRifle = Cast<AAH_Rifle>(DamageCauser);
			if (IsValid(PossibleRifle))
			{
				AAH_Character* RifleOwner = Cast<AAH_Character>(PossibleRifle->GetOwner());
				if (IsValid(RifleOwner))
				{
					TryToSpawnLoot();
				}

			}
		}

		SelfDestruction();
	}
}

void AAH_Bot::SelfDestruction()
{
	if (bIsExploded)
	{
		return;
	}

	bIsExploded = true;

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
	}

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true, ECC_Visibility);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 10, FColor::Red, false, 5.0f, 0.0f, 2.0f);
	}

	if (IsValid(MySpawner))
	{
		MySpawner->NotifyOfBotDeath();
	}

	PlayExplosionSound();

	Destroy();

}

void AAH_Bot::StartCountDown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsStartingCountDown)
	{
		return;
	}

	if (PlayerCharacter == OtherActor)
	{
		bIsStartingCountDown = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SelfDamage, this, &AAH_Bot::SelfDamage, 0.5f, true);
	}
}

void AAH_Bot::GiveXP(AActor* DamageCauser)
{
	AAH_Character* PossiblePlayer = Cast<AAH_Character>(DamageCauser);
	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
	}

	AAH_Rifle* PossibleRifle = Cast<AAH_Rifle>(DamageCauser);
	if (IsValid(PossibleRifle))
	{
		AAH_Character* PossibleRifleOwnerPlayer = Cast<AAH_Character>(PossibleRifle->GetOwner());
		if (IsValid(PossibleRifleOwnerPlayer) && PossibleRifleOwnerPlayer->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			PossibleRifleOwnerPlayer->GainUltimateXP(XPValue);
		}
	}
}

void AAH_Bot::SelfDamage()
{
	PlayTimerSound();
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

FVector AAH_Bot::GetNextPathPoint()
{
	if (!IsValid(PlayerCharacter))
	{
		return GetActorLocation();
	}
	
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	return GetActorLocation();
}

bool AAH_Bot::TryToSpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->AddEnemyDefeatedToCounter();
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

void AAH_Bot::PlayTimerSound()
{
	TimerSoundComponent->Play();
}

void AAH_Bot::PlayExplosionSound()
{
	if (!IsValid(ExplosionSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

// Called every frame
void AAH_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= MinDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= ForceMagnitude;

		BotMeshComponent->AddForce(ForceDirection, NAME_None, true);
	}

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), NextPathPoint, 30, 10, FColor::Emerald, false, 0, 0, 2);
	}

}

