// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_Projectile.generated.h"


class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Projectile : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ProjectileCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplosionEffect;

protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage|Radious")
	float ExplosionRadious;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HitDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

public:	
	// Sets default values for this actor's properties
	AAH_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyARadialDamage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Test")
	void BP_ApplyARadialDamage();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
