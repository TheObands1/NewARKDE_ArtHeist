// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AH_Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

// Sets default values
AAH_Projectile::AAH_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;

	ExplosionDamage = 20.0f;
	ExplosionRadious = 200.0f;
	HitDamage = 5.0f;
}

// Called when the game starts or when spawned
void AAH_Projectile::BeginPlay()
{
	Super::BeginPlay();
	//ApplyARadialDamage();
}

void AAH_Projectile::ApplyARadialDamage()
{
	if (IsValid(ExplosionEffect))
	{
		FVector OriginLocation = this->GetActorLocation();
		UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, OriginLocation, ExplosionRadious, DamageType, TArray<AActor*>(), this, (AController*)GetOwner(), false, ECC_Visibility);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, OriginLocation);
		Destroy();
	}
}

// Called every frame
void AAH_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Verifies is actor is valid
	if (IsValid(OtherActor))
	{
		APawn* OverlappedPawn = Cast<APawn>(OtherActor);
		//Verifies if actor is a pawn 
		if (IsValid(OverlappedPawn))
		{
			AActor* OwnerActor = this->GetOwner();
			if (IsValid(OwnerActor))
			{
				UGameplayStatics::ApplyDamage(OverlappedPawn, HitDamage, OwnerActor->GetInstigatorController(), this, DamageType);
			}
		}
	}

}

