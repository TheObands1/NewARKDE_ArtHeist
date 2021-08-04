// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_ControlsMenuWidget.generated.h"

/**
 * 
 */

class UAH_PauseMenuWidget;

UCLASS()
class ARKDE_ARTHEIST_API UAH_ControlsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pause Menu")
	UAH_PauseMenuWidget* PauseMenuReference;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Controls Menu")
	void GoBackToPauseMenu();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Controls Menu")
	void BP_GoBackToPauseMenu();
	
public: 

	void SetPauseMenuReference(UAH_PauseMenuWidget* NewPauseMenuReference) { PauseMenuReference = NewPauseMenuReference; };
};
