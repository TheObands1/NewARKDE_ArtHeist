// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_LevelMusic.generated.h"

class UAudioComponent;
class UBillboardComponent;
class AAH_GameMode;
class AAH_Character;

UCLASS()
class ARKDE_ARTHEIST_API AAH_LevelMusic : public AActor
{
	GENERATED_BODY()

protected:
	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MusicAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* SpecialSoundtrackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* MusicBillboardComponent;

protected:
	//Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	FName AlertParamName;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	AAH_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	AAH_Character* PlayerReference;
	
public:	
	// Sets default values for this actor's properties
	AAH_LevelMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StopLevelMusic();

	UFUNCTION()
	void ChangeLevelMusic(bool bIsAlert);

	UFUNCTION()
	void StopUltimateSoundtrackMusic();

	UFUNCTION()
	void StartUltimateSoundtrackMusic(int UltimateSoundtrackCode);

};
