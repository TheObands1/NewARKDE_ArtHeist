// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "AH_MainMenuButton.generated.h"

/**
 * 
 */
UENUM(Blueprintable)
enum class EAH_ButtonStyleType :uint8
{
	ButtonStyleType_Mouse     UMETA(DisplayName = "Selected by Mouse"),
	ButtonStyleType_KeySelected UMETA(DisplayName = "Selected by Keyboard"),
	ButtonStyleType_KeyNotSelected UMETA(DisplayName = "Not Selected by Keyboard"),
};


UCLASS()
class ARKDE_ARTHEIST_API UAH_MainMenuButton : public UButton
{
	GENERATED_BODY()


protected:
	//Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu Button")
	float CheckFocusRate;

	FTimerHandle TimerHandle_CheckFocus;

public:
	UAH_MainMenuButton();

protected:
	UFUNCTION(BlueprintCallable, Category = "Main Meny Button")
	void Start();

	void CheckFocus();

	UFUNCTION(BlueprintCallable, Category = "Main Menu Button")
	void SetButtonStyle(EAH_ButtonStyleType NewStyleType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu Button")
	void BP_SetButtonStyle(EAH_ButtonStyleType NewStyleType);


	
};
