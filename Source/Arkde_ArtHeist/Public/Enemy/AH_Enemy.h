// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AH_Character.h"
#include "AH_Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCallForHealthSignature, AActor*, Enemy, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotifyOfDeathSignature, AActor*, EnemyThatDied, float, Damage);
//I put AActor in the delegate because I haven't declared the class enemy yet, so it would have an non-existant parameter
/**
 * 
 */
class AAH_PathActor;
class AAH_Item;
class AAH_AIController;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Enemy : public AAH_Character
{
	GENERATED_BODY()

public:

	AAH_Enemy();

protected:
	//Objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
	AAH_PathActor* MyPath;

protected:
	//variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
	bool bIsEnemyAfraid;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Enemy Defeated Counter")
	bool bIsEnemyDefeated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ultimate XP Given")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 100.0, UIMax = 100.0))
	float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<AAH_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI| AI Controller")
	AAH_AIController* MyAIController;

public:
	//variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 100.0, UIMax = 100.0))
	int WeaponErrorMargin;

	UPROPERTY(BlueprintAssignable)
	FOnCallForHealthSignature OnCallForHealthDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnNotifyOfDeathSignature OnNotifyOfDeathDelegate;

protected:
	//functions
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnemyIsDamaged(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TryToSpawnLoot();

	bool TryToSpawnLoot();

public:
	//Functions
	bool GetLoopPath() { return bLoopPath; };

	int GetDirectionIndex() { return DirectionIndex; };

	float GetWaitingTimeOnPathPoint() { return WaitingTimeOnPathPoint; };

};
