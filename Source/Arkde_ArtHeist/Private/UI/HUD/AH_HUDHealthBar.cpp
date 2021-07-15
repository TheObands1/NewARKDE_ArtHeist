// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AH_HUDHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AH_HealthComponent.h"
#include "AH_Character.h"

void UAH_HUDHealthBar::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AAH_Character* PlayerCharacter = Cast<AAH_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			UAH_HealthComponent* PlayerHealthComponent = Cast<UAH_HealthComponent>(PlayerCharacter->GetHealthComponent());
			if (IsValid(PlayerHealthComponent))
			{
				PlayerHealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &UAH_HUDHealthBar::UpdateHealth);
			}
			
		}

	}
}
void UAH_HUDHealthBar::UpdateHealth(float Health, float MaxHealth)
{
	HealthPercent = Health / MaxHealth;

	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
}