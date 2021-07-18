// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDGameState.h"
#include "Core/AH_GameMode.h"

void UAH_HUDGameState::InitializeWidget()
{
	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &UAH_HUDGameState::OnVictory);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &UAH_HUDGameState::OnGameOver);
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UAH_HUDGameState::OnVictory()
{
	SetVisibility(ESlateVisibility::Visible);
	GameStateName = VictoryName;

	
}

void UAH_HUDGameState::OnGameOver()
{
	SetVisibility(ESlateVisibility::Visible);
	GameStateName = GameOverName;
}
