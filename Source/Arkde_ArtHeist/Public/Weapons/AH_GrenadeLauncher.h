// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/AH_Weapon.h"
#include "AH_GrenadeLauncher.generated.h"

/**
 * 
 */

class AAH_Projectile;

UCLASS()
class ARKDE_ARTHEIST_API AAH_GrenadeLauncher : public AAH_Weapon
{
	GENERATED_BODY()


public:

	AAH_GrenadeLauncher();
	
protected:
	//Varibales
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade Launcher")
	TSubclassOf<AAH_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;


protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
};
