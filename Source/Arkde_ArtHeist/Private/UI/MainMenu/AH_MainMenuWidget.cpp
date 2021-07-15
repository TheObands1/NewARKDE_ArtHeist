// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/AH_MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Core/AH_GameInstance.h"

void UAH_MainMenuWidget::NewGame()
{
	BP_NewGame();
	UGameplayStatics::OpenLevel(GetWorld(), GamePlayLevelName);
	UAH_GameInstance* GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->ResetData();
	}


}

void UAH_MainMenuWidget::ContinueGame()
{
	BP_ContinueGame();
	UGameplayStatics::OpenLevel(GetWorld(), GamePlayLevelName);

	UAH_GameInstance* GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->LoadData();
	}
}

void UAH_MainMenuWidget::QuitGame()
{
	BP_QuitGame();
	APlayerController* CurrentPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), CurrentPlayerController, EQuitPreference::Quit, false);
}
