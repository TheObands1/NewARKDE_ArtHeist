// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_LandMine.h"
#include "Arkde_ArtHeist/Arkde_ArtHeist.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AH_HealthComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AAH_LandMine::AAH_LandMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	LandMineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadMineMesh"));
	LandMineMesh->SetupAttachment(CustomRootComponent);

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
	ExplosionCollider->SetupAttachment(CustomRootComponent);
	ExplosionCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExplosionCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DetectionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionCollider"));
	DetectionCollider->SetupAttachment(CustomRootComponent);
	DetectionCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<UAH_HealthComponent>(TEXT("HealthComponent"));

	ForceImpulse = 1000;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->ImpulseStrength = ForceImpulse;

	ExplosionDamage = 200.0f;
	ExplosionRadious = 100.0f;

}

// Called when the game starts or when spawned
void AAH_LandMine::BeginPlay()
{
	Super::BeginPlay();
	DetectionCollider->OnComponentBeginOverlap.AddDynamic(this, &AAH_LandMine::Detect);
	ExplosionCollider->OnComponentBeginOverlap.AddDynamic(this, &AAH_LandMine::Explosion);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_LandMine::OnHealthChange);
}

void AAH_LandMine::Detect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		BP_Warning();
	}

}

void AAH_LandMine::Explosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (IsValid(ExplosionEffect))
		{
			FVector OriginLocation = this->GetActorLocation();
			UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, OriginLocation, ExplosionRadious, MyDamageType, TArray<AActor*>(), this, (AController*)GetOwner(), false, ECC_Visibility);
			BP_TestRadialDamage();
			RadialForceComponent->FireImpulse();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, OriginLocation);
			Destroy();
		}
		
	}
}

void AAH_LandMine::OnHealthChange(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		if (IsValid(DestroyingEffect))
		{
			FVector OriginLocation = this->GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyingEffect, OriginLocation);
			Destroy();
		}
		
	}
}

// Called every frame
void AAH_LandMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

