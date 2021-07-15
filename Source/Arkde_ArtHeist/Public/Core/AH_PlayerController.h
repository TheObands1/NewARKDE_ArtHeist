// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API AAH_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability to pause")
	bool bShouldThisPause;

	bool bIsGamePaused;

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:
	void PauseTheGame();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PauseGame(bool NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_UnPauseGame(bool NewState);
	
};
