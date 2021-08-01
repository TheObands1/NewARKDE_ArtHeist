// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/AH_Weapon.h"
#include "AH_Rifle.generated.h"

/**
 * 
 */

class UParticleSystem;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Rifle : public AAH_Weapon
{
	GENERATED_BODY()
	
public:

	AAH_Rifle();

protected:
	//Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace|Debug")
	bool bDrawLineTrace;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TraceEffect;

	

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CheckHitResult(FHitResult NewHitResult);
};
