// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDObjectivesContainer.h"
#include "UI/HUD/AH_HUDObjective.h"
#include "Core/AH_GameMode.h"

void UAH_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &UAH_HUDObjectivesContainer::UpdateObjectives);
	}
}

void UAH_HUDObjectivesContainer::InitializeObjectives(TArray<UAH_HUDObjective*> NewObjectives)
{
	for (UAH_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void UAH_HUDObjectivesContainer::UpdateObjectives(FName KeyTag)
{
	int ObjectiveIndex = -1;

	if (KeyTag == "Secret Room Key")
	{
		ObjectiveIndex = 0;
	}
	if (KeyTag == "Golden Room Key")
	{
		ObjectiveIndex = 1;
	}

	if (Objectives.IsValidIndex(ObjectiveIndex))
	{
		UAH_HUDObjective* CurrentObjective = Objectives[ObjectiveIndex];
		if (IsValid(CurrentObjective))
		{
			CurrentObjective->CompleteObjective();
		}
	}
}

