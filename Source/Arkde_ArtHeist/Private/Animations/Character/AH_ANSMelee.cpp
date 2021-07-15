// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/AH_ANSMelee.h"
#include "AH_Character.h"

void UAH_ANSMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor= MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		//Verifies is CharacterActor is of class AAH_Character
		AAH_Character* Character = Cast<AAH_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void UAH_ANSMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		//Verifies is CharacterActor is of class AAH_Character
		AAH_Character* Character = Cast<AAH_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::NoCollision);
		}
	}
}
