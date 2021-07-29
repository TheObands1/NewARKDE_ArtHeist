// Fill out your copyright notice in the Description page of Project Settings.


#include "Music/AH_LevelMusic.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"
#include "Core/AH_GameMode.h"

// Sets default value
AAH_LevelMusic::AAH_LevelMusic()
{
	MusicBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("MusicBillboardComponent"));
	RootComponent = MusicBillboardComponent;

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);

	AlertParamName = "Alert";

}

// Called when the game starts or when spawned
void AAH_LevelMusic::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &AAH_LevelMusic::StopLevelMusic);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &AAH_LevelMusic::StopLevelMusic);
		GameModeReference->OnAlertModeChangeDelegate.AddDynamic(this, &AAH_LevelMusic::ChangeLevelMusic);
	}

	
}

void AAH_LevelMusic::StopLevelMusic()
{
	MusicAudioComponent->Stop();
}

void AAH_LevelMusic::ChangeLevelMusic(bool bIsAlert)
{
	MusicAudioComponent->SetBoolParameter(AlertParamName, bIsAlert);
}

