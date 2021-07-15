// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AH_GameMode.generated.h"

/**
 * 
 */
class AAH_Character;
class AAH_SpectatingCamera;
UCLASS()
class ARKDE_ARTHEIST_API AAH_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;


	//There is no need for VisibleAnywhere, As the GameMode is an abstract component
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AAH_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AAH_SpectatingCamera* GameOverCamera;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(AAH_Character* Character, AAH_SpectatingCamera* SpectatingCamera);


public:

	UFUNCTION()
	void Victory(AAH_Character* Character);
	
	UFUNCTION()
	void GameOver(AAH_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(AAH_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(AAH_Character* Character);
};
