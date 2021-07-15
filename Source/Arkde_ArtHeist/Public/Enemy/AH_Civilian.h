// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AH_Character.h"
#include "AH_Civilian.generated.h"

/**
 * 
 */

class AAH_AICivilianController;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Civilian : public AAH_Character
{
	GENERATED_BODY()


public:
	AAH_Civilian();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bCanSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsCivilianDamaged;

	UPROPERTY(BlueprintReadOnly, Category = "AI| AI Controller")
	AAH_AICivilianController* MyOwnAIController;

protected:
	//functions
	virtual void BeginPlay() override;

	UFUNCTION()
	void CivilianIsDamaged(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

};
