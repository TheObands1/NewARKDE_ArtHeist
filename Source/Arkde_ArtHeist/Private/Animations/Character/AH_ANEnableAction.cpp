// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/AH_ANEnableAction.h"
#include "AH_Character.h"

void UAH_ANEnableAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		//Verifies is CharacterActor is of class AAH_Character
		AAH_Character* Character = Cast<AAH_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeState(false);
			Character->ResetCombo();
		}
	}
}
