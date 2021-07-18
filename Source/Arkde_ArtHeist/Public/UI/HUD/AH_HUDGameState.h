// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_HUDGameState.generated.h"


class AAH_GameMode;
/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_HUDGameState : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	AAH_GameMode* GameModeReference;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	FName GameStateName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FName VictoryName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FName GameOverName;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void OnVictory();
	
	UFUNCTION()
	void OnGameOver();
	
};
