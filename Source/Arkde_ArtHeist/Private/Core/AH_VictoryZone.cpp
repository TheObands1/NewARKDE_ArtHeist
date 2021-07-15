// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AH_VictoryZone.h"
#include "Components/BoxComponent.h"
#include "AH_Character.h"
#include "Core/AH_GameMode.h"

// Sets default values
AAH_VictoryZone::AAH_VictoryZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VictoryZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryZoneComponent"));
	RootComponent = VictoryZoneComponent;
	VictoryZoneComponent->SetBoxExtent(FVector(100.0f));

}

// Called when the game starts or when spawned
void AAH_VictoryZone::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());
	
}

// Called every frame
void AAH_VictoryZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_VictoryZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (IsValid(OtherActor) && IsValid(GameModeReference))
	{
		AAH_Character* AHCharacter = Cast<AAH_Character>(OtherActor);
		
		if (IsValid(AHCharacter) && AHCharacter->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			GameModeReference->Victory(AHCharacter);
		}
	}
}

