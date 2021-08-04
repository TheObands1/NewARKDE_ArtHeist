// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/AH_PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Core/AH_GameInstance.h"
#include "AH_Character.h"
#include "Core/AH_PlayerController.h"

void UAH_PauseMenuWidget::ResumeGame()
{
	BP_ResumeGame();
	AAH_PlayerController* PossiblePlayerController = Cast<AAH_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(PossiblePlayerController))
	{
		PossiblePlayerController->PauseTheGame();
	}
}

void UAH_PauseMenuWidget::Options()
{
	BP_Options();
}

void UAH_PauseMenuWidget::SaveAndExit()
{
	BP_SaveAndExit();
	UAH_GameInstance* GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}

void UAH_PauseMenuWidget::ShowControls()
{
	BP_ShowControls();
}
