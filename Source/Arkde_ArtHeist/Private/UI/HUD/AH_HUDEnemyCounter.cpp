// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDEnemyCounter.h"
#include "Core/AH_GameInstance.h"

void UAH_HUDEnemyCounter::InitializeWidget()
{
	GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnEnemyKilledDelegate.AddDynamic(this, &UAH_HUDEnemyCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetEnemiesDefeatedCounter());
	}
}

void UAH_HUDEnemyCounter::UpdateCounter(int EnemyDefeatedCounter)
{
	EnemiesDefeated = EnemyDefeatedCounter;
}
