// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_LandMine.generated.h"

class UStatichMeshComponent;
class USphereComponent;
class UAH_HealthComponent;
class UParticleSystem;
class URadialForceComponent;
class USoundCue;


UCLASS()
class ARKDE_ARTHEIST_API AAH_LandMine : public AActor
{
	GENERATED_BODY()
	
protected:
	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LandMineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ExplosionCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* DetectionCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAH_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestroyingEffect;


protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ExplosionRadious;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Force")
	float ForceRadious;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Force")
	float ForceImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> MyDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* ExplosionSound;

public:	
	// Sets default values for this actor's properties
	AAH_LandMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayExplosionSound();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Warning();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Test")
	void BP_TestRadialDamage();

	//Has to have macro because its binded to a delegate
	UFUNCTION()
	void Detect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Explosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
