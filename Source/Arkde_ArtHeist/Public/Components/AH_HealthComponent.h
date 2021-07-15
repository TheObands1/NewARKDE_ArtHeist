// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AH_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UAH_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdateSignature, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, AActor*, DamageCauser);
class AActor;

UCLASS( ClassGroup=(ART_HEIST), meta=(BlueprintSpawnableComponent) )
class ARKDE_ARTHEIST_API UAH_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
	AActor* MyOwner;

	FTimerHandle TimerHanlde_UpdateInitialHealth;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdateSignature OnHealthUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDeadSignature OnDeadDelegate;

public:	
	// Sets default values for this component's properties
	UAH_HealthComponent();

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	bool IsDead() const { return bIsDead; };

	float GetHealth();

	float GetMaxHealth();

	bool TryToAddHealth(float HealthToAdd);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void UpdateInitialHealth();
};
