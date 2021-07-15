// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_ExplodingBarrel.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AH_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "AH_Character.h"

// Sets default values
AAH_ExplodingBarrel::AAH_ExplodingBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	ExplodingBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplodingBarrelMesh"));
	ExplodingBarrelMesh->SetupAttachment(CustomRootComponent);

	DetectionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionCollider"));
	DetectionCollider->SetupAttachment(CustomRootComponent);

	DamageDetectionCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageDetectionCollider"));
	DetectionCollider->SetupAttachment(CustomRootComponent);

	HealthComponent = CreateDefaultSubobject<UAH_HealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void AAH_ExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
	DetectionCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectionCollider->OnComponentBeginOverlap.AddDynamic(this, &AAH_ExplodingBarrel::DetectNearbyCharacter);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_ExplodingBarrel::OnHealthChange);
	
}

void AAH_ExplodingBarrel::DetectNearbyCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->Burn();
		}
	}
}


void AAH_ExplodingBarrel::OnHealthChange(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		if (IsValid(ExplosionEffect))
		{
			FVector OriginLocation = this->GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, OriginLocation);
			DetectionCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			Destroy();
		}

	}
}

// Called every frame
void AAH_ExplodingBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

