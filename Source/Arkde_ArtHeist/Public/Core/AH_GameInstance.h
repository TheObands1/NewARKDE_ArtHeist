// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AH_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	int EnemiesDefeatedCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	FString SaveSlotName;

public:

	UAH_GameInstance(); //Constructor

	int GetEnemiesDefeatedCounter() { return EnemiesDefeatedCounter; };

	void SetEnemiesDefeatedCounter(int NewValue) { EnemiesDefeatedCounter = NewValue; };

	void AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintCallable)
	void SaveData();

	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable)
	void ResetData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SaveData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_LoadData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ResetData();



};
