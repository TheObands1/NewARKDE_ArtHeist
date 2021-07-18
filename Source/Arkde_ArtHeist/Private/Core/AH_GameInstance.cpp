// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AH_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/AH_SaveGame.h"

UAH_GameInstance::UAH_GameInstance()
{
	SaveSlotName = "AH_SavedData";
}

void UAH_GameInstance::AddEnemyDefeatedToCounter()
{
	EnemiesDefeatedCounter++;
	OnEnemyKilledDelegate.Broadcast(EnemiesDefeatedCounter);
	BP_AddEnemyDefeatedToCounter();
}

void UAH_GameInstance::SaveData()
{
	BP_SaveData();
	USaveGame* SaveGameObject = nullptr;
	bool bExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	if (bExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);

	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(UAH_SaveGame::StaticClass());
	}

	if (IsValid(SaveGameObject))
	{
		UAH_SaveGame* SaveFile = Cast<UAH_SaveGame>(SaveGameObject);

		if (IsValid(SaveFile))
		{
			SaveFile->SetEnemiesDefeatedCounterInfo(EnemiesDefeatedCounter);
			UGameplayStatics::SaveGameToSlot(SaveFile, SaveSlotName, 0);
		}
	}
}

void UAH_GameInstance::LoadData()
{
	BP_LoadData();
	USaveGame* SaveGameObject = nullptr;
	bool bExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	if (bExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);

	}

	if (IsValid(SaveGameObject))
	{
		UAH_SaveGame* SaveFile = Cast<UAH_SaveGame>(SaveGameObject);

		if (IsValid(SaveFile))
		{
			EnemiesDefeatedCounter = SaveFile->GetEnemiesDefeatedCounterInfo();
		}
	}

}

void UAH_GameInstance::ResetData()
{
	EnemiesDefeatedCounter = 0;
	BP_ResetData();
}
