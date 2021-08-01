// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_BotSpawner.h"
#include "Components/BillboardComponent.h"
#include "Enemy/AH_Bot.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAH_BotSpawner::AAH_BotSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnerBillboardComponent"));
	RootComponent = SpawnerBillboardComponent;

	bIsActive = true;
	MaxBotCounter = 3;
	TimeToSpawn = 1.0f;

}

// Called when the game starts or when spawned
void AAH_BotSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &AAH_BotSpawner::SpawnBot, TimeToSpawn, true);
	
}

void AAH_BotSpawner::SpawnBot()
{
	if (!bIsActive)
	{
		return;
	}

	if (CurrentBotsCounter >= MaxBotCounter)
	{
		return;
	}

	if (IsValid(BotClass))
	{
		const FVector LocalSpawnPoint = GetSpawnPoint();
		const FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);

		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//For Spawning actor and, after the game starts, set the BotSpawner that created it:
		//AAH_Bot* NewBot = GetWorld()->SpawnActor<AAH_Bot>(BotClass, SpawnPoint, FRotator::ZeroRotator, SpawnParameter);
		
		//For Spawning the actor but setting some characteristics, such as the BotSpawner that created it, first before it actually appears in world:
		const FTransform BotTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);
		AAH_Bot* NewBot = GetWorld()->SpawnActorDeferred<AAH_Bot>(BotClass, BotTransform);

		if (IsValid(NewBot))
		{
			NewBot->SetBotSpawner(this);
		}

		NewBot->FinishSpawning(BotTransform);

		CurrentBotsCounter++;

	}
}

FVector AAH_BotSpawner::GetSpawnPoint()
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

