// Fill out your copyright notice in the Description page of Project Settings.


#include "Music/AH_MainMenuMusic.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AAH_MainMenuMusic::AAH_MainMenuMusic()
{
	MusicBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("MusicBillboardComponent"));
	RootComponent = MusicBillboardComponent;

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAH_MainMenuMusic::BeginPlay()
{
	Super::BeginPlay();
	
}
