// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_HUDObjectivesContainer.generated.h"

/**
 * 
 */

class AAH_GameMode;
class UAH_HUDObjective;


UCLASS()
class ARKDE_ARTHEIST_API UAH_HUDObjectivesContainer : public UUserWidget
{
	GENERATED_BODY()

protected:
	//References
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	AAH_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	TArray<UAH_HUDObjective*> Objectives;

public:
	
	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void InitializeObjectives(TArray<UAH_HUDObjective*> NewObjectives);

	UFUNCTION()
	void UpdateObjectives(FName KeyTag);
	
};
