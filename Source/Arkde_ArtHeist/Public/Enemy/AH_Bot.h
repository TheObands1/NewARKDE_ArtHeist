// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AH_Bot.generated.h"


class AAH_Character;
class AAH_Item;
class AAH_BotSpawner;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class UAH_HealthComponent;
class UStaticMeshComponent;
class UAH_GameInstance;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Bot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAH_Bot();

public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAH_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SelfDestructionDetectorComponent;

protected:
	//Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
	float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
	float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Ultimate XP Given")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 100.0, UIMax = 100.0))
	float LootProbability;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Movement")
	bool bIsExploded;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Movement")
	bool bIsStartingCountDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Navigation")
	FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	AAH_Character* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	AAH_BotSpawner* MySpawner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<AAH_Item> LootItemClass;

	UMaterialInstanceDynamic* BotMaterial;

	UAH_GameInstance* GameInstanceReference;

	FTimerHandle TimerHandle_SelfDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void StartCountDown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	void SelfDamage();

	void SelfDestruction();

	UFUNCTION(BlueprintCallable, Category = "Bot Navigation")
	FVector GetNextPathPoint();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TryToSpawnLoot();

	bool TryToSpawnLoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBotSpawner(AAH_BotSpawner* NewSpawner) { MySpawner = NewSpawner; };
};
