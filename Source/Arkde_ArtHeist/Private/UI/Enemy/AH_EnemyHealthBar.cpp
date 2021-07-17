// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/AH_EnemyHealthBar.h"

void UAH_EnemyHealthBar::UpdateHealth(float Health, float MaxHealth)
{
	HealthPercent = Health / MaxHealth;
}
