// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDUltimate.h"
#include "Kismet/GameplayStatics.h"
#include "AH_Character.h"

void UAH_HUDUltimate::InitializeWidget()
{
	UltimateColor = UltimateDisabledColor;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AAH_Character* PlayerCharacter = Cast<AAH_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnUltimateUpdateDelegate.AddDynamic(this, &UAH_HUDUltimate::UpdateUltimateValue);
			PlayerCharacter->OnUltimateStatusDelegate.AddDynamic(this, &UAH_HUDUltimate::UpdateUltimateStatus);
		}

	}
}

void UAH_HUDUltimate::UpdateUltimateValue(float CurrentUltimateXP, float MaxUltimateXP)
{
	UltimatePercent = CurrentUltimateXP / MaxUltimateXP;
}

void UAH_HUDUltimate::UpdateUltimateStatus(bool bIsAvailable)
{
	UltimateColor = bIsAvailable ? UltimateEnabledColor : UltimateDisabledColor;
}
