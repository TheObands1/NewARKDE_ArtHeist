// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AH_GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertModeChangeSignature, bool, bIsInAlertMode);

/**
 * 
 */
class AAH_Character;
class AAH_Enemy;
class AAH_SpectatingCamera;
class USoundCue;
UCLASS()
class ARKDE_ARTHEIST_API AAH_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	float TimeToGoBackToMenuAfterVictory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	float TimeToGoBackToMenuAfterGameOver;

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	bool bIsInAlertMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameFlow")
	FName MainMenuMapName;

	//There is no need for VisibleAnywhere, As the GameMode is an abstract component
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AAH_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AAH_SpectatingCamera* GameOverCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* VictoryMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* GameOverMusic;

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	TArray<AAH_Enemy*> LevelEnemies;

	FTimerHandle TimerHandle_BackToMainMenu;

public:
	//Constructor
	AAH_GameMode();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(AAH_Character* Character, AAH_SpectatingCamera* SpectatingCamera);

	void PlayMusic(USoundCue* SoundCueToPlay);

	void SetupEnemiesInLevel();

public: 

	UPROPERTY(BlueprintAssignable)
	FOnKeyAddedSignature OnKeyAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChangeSignature OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChangeSignature OnGameOverDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAlertModeChangeSignature OnAlertModeChangeDelegate;

public:

	UFUNCTION()
	void Victory(AAH_Character* Character);
	
	UFUNCTION()
	void GameOver(AAH_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(AAH_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(AAH_Character* Character);

	void BackToMainMenu();

	void AddKeyToCharacter(AAH_Character* KeyOwner, FName KeyTag);

	void CheckAlertMode();
};
