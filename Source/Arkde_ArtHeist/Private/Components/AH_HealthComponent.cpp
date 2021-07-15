// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AH_HealthComponent.h"

// Sets default values for this component's properties
UAH_HealthComponent::UAH_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
}


// Called when the game starts
void UAH_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	MyOwner = GetOwner();

	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UAH_HealthComponent::TakingDamage);

		//Because OnTakeAnyDamage is a delegate, the function TakingDamage must recieve the same parameters as said delegate
	}
	//To set a timer: Timer, object from which the function will be called, the function to called, the time it will take to call it each time, and a bool asking if it wants to be looped
	GetWorld()->GetTimerManager().SetTimer(TimerHanlde_UpdateInitialHealth, this, &UAH_HealthComponent::UpdateInitialHealth, 0.2f, false);
	
}

void UAH_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage < 0.0f)
	{
		return;
	}
	
	Health = FMath::Clamp((Health - Damage), 0.0f, MaxHealth);

	if (Health == 0.0f)
	{
		bIsDead = true;
		OnDeadDelegate.Broadcast(DamageCauser);
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);

	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("My Health is %s"), *FString::SanitizeFloat(Health));
	}
}

void UAH_HealthComponent::UpdateInitialHealth()
{
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);
}

float UAH_HealthComponent::GetHealth()
{
	return Health;
}

float UAH_HealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool UAH_HealthComponent::TryToAddHealth(float HealthToAdd)
{
	if (bIsDead)
	{
		return false;
	}

	if (Health == MaxHealth)
	{
		return false;
	}

	Health = FMath::Clamp((Health + HealthToAdd), 0.0f, MaxHealth);
	OnHealthUpdateDelegate.Broadcast(Health, MaxHealth);

	return true;
}

