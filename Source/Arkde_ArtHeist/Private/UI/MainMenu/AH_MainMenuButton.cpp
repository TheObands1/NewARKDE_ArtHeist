// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/AH_MainMenuButton.h"

UAH_MainMenuButton::UAH_MainMenuButton()
{
	CheckFocusRate = 0.1f;
}

void UAH_MainMenuButton::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckFocus, this, &UAH_MainMenuButton::CheckFocus, CheckFocusRate, true);
}

void UAH_MainMenuButton::CheckFocus()
{
	if (IsHovered())
	{
		SetKeyboardFocus();
		SetButtonStyle(EAH_ButtonStyleType::ButtonStyleType_Mouse);
	}
	else
	{
		EAH_ButtonStyleType StyleTypeSelected = HasKeyboardFocus() ? EAH_ButtonStyleType::ButtonStyleType_KeySelected : EAH_ButtonStyleType::ButtonStyleType_KeyNotSelected;

	}
}

void UAH_MainMenuButton::SetButtonStyle(EAH_ButtonStyleType NewStyleType)
{
	BP_SetButtonStyle(NewStyleType);
}
