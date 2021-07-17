// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	float HealthPercent;

public:
	UFUNCTION()
	void UpdateHealth(float Health, float MaxHealth);
};


