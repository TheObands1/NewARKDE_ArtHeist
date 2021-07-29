// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AH_Character.h"
#include "Weapons/AH_Rifle.h"
#include "AH_HealingBot.generated.h"


class UStaticMeshComponent;
class UAH_HealthComponent;
class USphereComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class AAH_Enemy;
class AAH_Item;
class UAH_GameInstance;
class USoundCue;

UCLASS()
class ARKDE_ARTHEIST_API AAH_HealingBot : public APawn
{
	GENERATED_BODY()

public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealingBotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAH_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* EnemyToHealDetectorComponent;

protected:
	//variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Healing", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float UpSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Healing", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AmountOfHealthToAdd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Behavior")
	float MinUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Behavior")
	float MaxUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curing Behavior")
	float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingBot Movement")
	float DefaultForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingBot Movement")
	float CuringForceMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Bot Ultimate XP Given")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 100.0, UIMax = 100.0))
	float LootProbability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default Behavior")
	bool bIsGoingUp;

	bool bIsInCuringStatus;

	bool bIsInDefaultStatus;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing|Enemies to Heal")
	TArray<AAH_Enemy*> EnemiesInWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<AAH_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "HealingBot Navigation")
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DeathExplosionEffect;

	UPROPERTY(BlueprintReadOnly)
	AAH_Enemy* CurrentEnemyToCure;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* ExplosionSound;

	UMaterialInstanceDynamic* BotMaterial;

	UAH_GameInstance* GameInstanceReference;
	
public:
	// Sets default values for this pawn's properties
	AAH_HealingBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ChangeToCuringStatus(AActor* Enemy, float Damage);

	UFUNCTION()
	void DeleteEnemyFromEnemiesInWorld(AActor* Enemy, float Damage);

	UFUNCTION()
	void TakingDamage(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void CureEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_DefaultBehavior();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowCuringEffect(FVector EffectLocation, FRotator EffectRotation);

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TryToSpawnLoot();

	void SetupEnemiesInWorld();

	void PlayExplosionSound();

	UFUNCTION(BlueprintCallable, Category = "HealingBot Navigation")
	FVector GetNextPathPoint();

	bool TryToSpawnLoot();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
