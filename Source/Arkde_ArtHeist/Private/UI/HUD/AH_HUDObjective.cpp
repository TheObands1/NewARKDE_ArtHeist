// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDObjective.h"

void UAH_HUDObjective::CompleteObjective()
{
	bIsObjectiveCompleted = true;
	CurrentColor = CompletedColor;
}
