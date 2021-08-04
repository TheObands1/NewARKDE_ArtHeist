// Fill out your copyright notice in the Description page of Project Settings.


#include "Music/AH_LevelMusic.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"
#include "Core/AH_GameMode.h"
#include "AH_Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default value
AAH_LevelMusic::AAH_LevelMusic()
{
	MusicBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("MusicBillboardComponent"));
	RootComponent = MusicBillboardComponent;

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);

	SpecialSoundtrackComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpecialSoundtrackComponent"));
	SpecialSoundtrackComponent->SetupAttachment(RootComponent);

	AlertParamName = "Alert";

}

// Called when the game starts or when spawned
void AAH_LevelMusic::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());
	PlayerReference = Cast<AAH_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &AAH_LevelMusic::StopLevelMusic);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &AAH_LevelMusic::StopLevelMusic);
		GameModeReference->OnAlertModeChangeDelegate.AddDynamic(this, &AAH_LevelMusic::ChangeLevelMusic);
	}

	if (IsValid(PlayerReference))
	{
		PlayerReference->OnUltimateStartDelegate.AddDynamic(this, &AAH_LevelMusic::StartUltimateSoundtrackMusic);
		PlayerReference->OnUltimateStopDelegate.AddDynamic(this, &AAH_LevelMusic::StopUltimateSoundtrackMusic);
	}
}

void AAH_LevelMusic::StopLevelMusic()
{
	MusicAudioComponent->Stop();
}

void AAH_LevelMusic::ChangeLevelMusic(bool bIsAlert)
{
	//if que primero verifique si estoy en ultimate. Si no estoy en ultimate, 
	MusicAudioComponent->SetBoolParameter(AlertParamName, bIsAlert);
}

void AAH_LevelMusic::StopUltimateSoundtrackMusic()
{
	SpecialSoundtrackComponent->Stop();
	MusicAudioComponent->Play();
}

void AAH_LevelMusic::StartUltimateSoundtrackMusic(int UltimateSoundtrackCode)
{
	StopLevelMusic();
	SpecialSoundtrackComponent->Play();
	SpecialSoundtrackComponent->SetIntParameter("UltimateSoundtrack", UltimateSoundtrackCode);

}

