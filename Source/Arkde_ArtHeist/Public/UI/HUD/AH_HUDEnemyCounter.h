// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_HUDEnemyCounter.generated.h"


class UAH_GameInstance;
/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_HUDEnemyCounter : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int EnemiesDefeated;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	UAH_GameInstance* GameInstanceReference;
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateCounter(int EnemyDefeatedCounter);
};
