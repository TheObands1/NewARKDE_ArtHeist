// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_HealingBot.h"
#include "Enemy/AH_Enemy.h"
#include "Arkde_ArtHeist/Arkde_ArtHeist.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Components/AH_HealthComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/AH_Item.h"
#include "Math/UnrealMathUtility.h"
#include "Core/AH_GameInstance.h"



// Sets default values
AAH_HealingBot::AAH_HealingBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealingBotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealingBotMeshComponent"));
	HealingBotMeshComponent->SetCanEverAffectNavigation(false);
	HealingBotMeshComponent->SetSimulatePhysics(true);
	RootComponent = HealingBotMeshComponent;

	EnemyToHealDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyToHealDetectorComponent"));
	EnemyToHealDetectorComponent->SetupAttachment(RootComponent);
	EnemyToHealDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnemyToHealDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EnemyToHealDetectorComponent->SetSphereRadius(150);

	HealthComponent = CreateDefaultSubobject<UAH_HealthComponent>(TEXT("HealthComponent"));
	bIsInDefaultStatus = true;
	AmountOfHealthToAdd = 20.0f;
	MinDistanceToTarget = 100.0f;
	DefaultForceMagnitude = 100.0f;
	CuringForceMagnitude = 1000.0F;
	UpSpeed = 10.0f;
	XPValue = 10.0f;
	LootProbability = 100.0f;
}

// Called when the game starts or when spawned
void AAH_HealingBot::BeginPlay()
{
	Super::BeginPlay();
	SetupEnemiesInWorld();
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_HealingBot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AAH_HealingBot::GiveXP);
	EnemyToHealDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_HealingBot::CureEnemy);
	BotMaterial = HealingBotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, HealingBotMeshComponent->GetMaterial(0));
	GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());
	
}

void AAH_HealingBot::CureEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsInCuringStatus)
	{
		if (IsValid(CurrentEnemyToCure))
		{
			if (OtherActor == CurrentEnemyToCure)
			{
				FVector EffectLocation = CurrentEnemyToCure->GetActorLocation();
				FRotator EffectRotation = CurrentEnemyToCure->GetActorRotation();
				BP_ShowCuringEffect(EffectLocation, EffectRotation);
				bool bDoesEnemyNeedMoreHealth = CurrentEnemyToCure->TryToAddHealth(AmountOfHealthToAdd);
				if (!bDoesEnemyNeedMoreHealth)
				{
					bIsInCuringStatus = false;
					bIsInDefaultStatus = true;
				}
			}
		}
	}
}

FVector AAH_HealingBot::GetNextPathPoint()
{
	if (!IsValid(CurrentEnemyToCure))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), CurrentEnemyToCure);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	return GetActorLocation();
}

void AAH_HealingBot::GiveXP(AActor* DamageCauser)
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

void AAH_HealingBot::SetupEnemiesInWorld()
{
	TArray<AActor*> ActorsInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAH_Enemy::StaticClass(), ActorsInWorld);

	//StaticClass allows me to get a TSubClass of a class without it changing, hence it is "static". In this case, I don't need this 
	//attribute as a variable because it won't change

	if (ActorsInWorld.Num() > 0)
	{
		for (int i = 0; i < ActorsInWorld.Num(); i++)
		{
			AAH_Enemy* PossibleEnemy = Cast<AAH_Enemy>(ActorsInWorld[i]);
			if (IsValid(PossibleEnemy))
			{
				//Add every actor who is truly an Enemy
				EnemiesInWorld.Add(PossibleEnemy);
			}
		}
	}
	if (EnemiesInWorld.Num() > 0)
	{
		for (int i = 0; i < EnemiesInWorld.Num(); i++)
		{
			EnemiesInWorld[i]->OnCallForHealthDelegate.AddDynamic(this, &AAH_HealingBot::ChangeToCuringStatus);
			EnemiesInWorld[i]->OnNotifyOfDeathDelegate.AddDynamic(this, &AAH_HealingBot::DeleteEnemyFromEnemiesInWorld);
		}
	}
}


void AAH_HealingBot::ChangeToCuringStatus(AActor* Enemy, float Damage)
{
	AAH_Enemy* PossibleEnemy = Cast<AAH_Enemy>(Enemy);
	if (IsValid(PossibleEnemy))
	{
		for (int i = 0; i < EnemiesInWorld.Num(); i++)
		{
			if (EnemiesInWorld[i] == PossibleEnemy)
			{
				CurrentEnemyToCure = EnemiesInWorld[i];
				bIsInDefaultStatus = false;
				bIsInCuringStatus = true;
			}
		}
	}
}

void AAH_HealingBot::DeleteEnemyFromEnemiesInWorld(AActor* Enemy, float Damage)
{
	AAH_Enemy* EnemyToDelete = Cast<AAH_Enemy>(Enemy);
	bool bIsEnemyInWorld = false;
	int PositionOfEnemyInArray = 0;
	if (IsValid(EnemyToDelete))
	{
		for (int i = 0; i < EnemiesInWorld.Num(); i++)
		{
			if (EnemiesInWorld[i] == EnemyToDelete)
			{
				bIsEnemyInWorld = true;
			    PositionOfEnemyInArray = i;
			}
		}
		
		if (bIsEnemyInWorld)
		{
			EnemiesInWorld[PositionOfEnemyInArray]->OnCallForHealthDelegate.RemoveDynamic(this, &AAH_HealingBot::ChangeToCuringStatus);
			EnemiesInWorld[PositionOfEnemyInArray]->OnNotifyOfDeathDelegate.RemoveDynamic(this, &AAH_HealingBot::DeleteEnemyFromEnemiesInWorld);
			EnemiesInWorld.Remove(EnemyToDelete);
			bIsInCuringStatus = false;
			bIsInDefaultStatus = true;
		}
	}
}

void AAH_HealingBot::TakingDamage(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		if (IsValid(DeathExplosionEffect))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathExplosionEffect, GetActorLocation(), GetActorRotation());
		}

		if (IsValid(GameInstanceReference))
		{
			GameInstanceReference->AddEnemyDefeatedToCounter();
		}

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

		Destroy();

	}
}
bool AAH_HealingBot::TryToSpawnLoot()
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
// Called every frame
void AAH_HealingBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInDefaultStatus)
	{
		//Move Randomly

		FVector ForceDirection = FMath::VRand();
		ForceDirection.Normalize();
		ForceDirection *= DefaultForceMagnitude;
		HealingBotMeshComponent->AddForce(ForceDirection, NAME_None, true);
		BP_DefaultBehavior();
	}

	if (bIsInCuringStatus)
	{
		if (IsValid(CurrentEnemyToCure))
		{
			NextPathPoint = GetNextPathPoint();

			UE_LOG(LogTemp, Warning, TEXT("Current enemy is valid"));

			float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

			if (DistanceToTarget <= MinDistanceToTarget)
			{
				NextPathPoint = GetNextPathPoint();
			}
			else
			{
				FVector ForceDirection = NextPathPoint - GetActorLocation();
				ForceDirection.Normalize();
				ForceDirection *= CuringForceMagnitude;

				HealingBotMeshComponent->AddForce(ForceDirection, NAME_None, true);
			}

			if (bDebug)
			{
				DrawDebugSphere(GetWorld(), NextPathPoint, 30, 10, FColor::Black, false, 0, 0, 2);
			}
		}
	}
}

