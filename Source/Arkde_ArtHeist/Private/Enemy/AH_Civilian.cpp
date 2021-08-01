// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_Civilian.h"
#include "Enemy/Controller/AH_AICivilianController.h"
#include "Components/AH_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISense_Damage.h"
#include "AH_Character.h"

AAH_Civilian::AAH_Civilian()
{

}

void AAH_Civilian::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_Civilian::CivilianIsDamaged);

	MyOwnAIController= Cast<AAH_AICivilianController>(GetController());

}

void AAH_Civilian::CivilianIsDamaged(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (CurrentHealthComponent->IsDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeArmDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (IsValid(MyOwnAIController))
		{
			MyOwnAIController->UnPossess();
			MyOwnAIController->Destroy();
		}
	}
	else
	{
		bIsCivilianDamaged = true;
		if (IsValid(MyOwnAIController))
		{
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, DamageCauser->GetActorLocation(), FVector::ZeroVector);
		}

	}
}

