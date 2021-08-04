// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_PauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	FName  MainMenuLevelName;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void Options();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void SaveAndExit();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_ResumeGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_Options();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_SaveAndExit();

	UFUNCTION(BlueprintImplementableEvent, Category = "Controls Menu")
	void BP_ShowControls();

public:
	UFUNCTION(BlueprintCallable, Category = "Controls Menu")
	void ShowControls();


};
