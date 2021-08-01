// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AH_KeySpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/AH_DoorKey.h"
#include "Components/BillboardComponent.h"

// Sets default values
AAH_KeySpawner::AAH_KeySpawner()
{
	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnerBillboardComponent"));
	RootComponent = SpawnerBillboardComponent;
	KeyTagOfSpawnedKey = "Secret Room Key";
}

// Called when the game starts or when spawned
void AAH_KeySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnKey();
	
}

void AAH_KeySpawner::SpawnKey()
{
	if (IsValid(DoorKeyClass))
	{
		const FVector LocalSpawnPoint = GetSpawnPoint();
		const FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);

		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//For Spawning actor and, after the game starts, set the BotSpawner that created it:
		//AAH_DoorKey* NewDoorKey  = GetWorld()->SpawnActor<AAH_DoorKey>(DoorKeyClass, SpawnPoint, FRotator::ZeroRotator, SpawnParameter);

		//For Spawning the actor but setting some characteristics, such as the BotSpawner that created it, first before it actually appears in world:
		const FTransform DoorKeyTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);
		AAH_DoorKey* NewDoorKey = GetWorld()->SpawnActorDeferred<AAH_DoorKey>(DoorKeyClass, DoorKeyTransform);

		if (IsValid(NewDoorKey))
		{
			NewDoorKey->SetKeyTag(KeyTagOfSpawnedKey);
		}

		NewDoorKey->FinishSpawning(DoorKeyTransform);
	}
}

FVector AAH_KeySpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		const int IndexSelected = FMath::RandRange(0, (SpawnPoints.Num() - 1));

		return SpawnPoints[IndexSelected];
	}
	else
	{
		return GetActorLocation();

	}
}


