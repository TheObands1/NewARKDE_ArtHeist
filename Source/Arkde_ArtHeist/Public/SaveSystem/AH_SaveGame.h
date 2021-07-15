// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AH_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
	int EnemiesDefeatedCounter;

public:
	UFUNCTION(BlueprintCallable)
	int GetEnemiesDefeatedCounterInfo() { return EnemiesDefeatedCounter; };

	UFUNCTION(BlueprintCallable)
	void SetEnemiesDefeatedCounterInfo(int NewValue) { EnemiesDefeatedCounter = NewValue; };
};
