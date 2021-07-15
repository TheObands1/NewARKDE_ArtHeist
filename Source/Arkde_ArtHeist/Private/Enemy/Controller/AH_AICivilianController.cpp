// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/AH_AICivilianController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "AH_Character.h"
#include "Enemy/AH_Civilian.h"
#include "Perception/AISense_Damage.h"

AAH_AICivilianController::AAH_AICivilianController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	CanSeePlayerParameterName = "bCanSeePlayer";
	IsReceivingDamageParameterName = "bIsReceivingDamage";
}

void AAH_AICivilianController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CivilianBehaviorTree))
	{
		RunBehaviorTree(CivilianBehaviorTree);
		bWasBehaviorTreeLoadedSuccessfully = true;
	}

	MyCivilian = Cast<AAH_Civilian>(K2_GetPawn());

	if (IsValid(MyCivilian) && bWasBehaviorTreeLoadedSuccessfully)
	{
		MyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	}

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAH_AICivilianController::UpdateSenses);

}

void AAH_AICivilianController::UpdateSenses(const TArray<AActor*>& UpdatedActors)
{
	if (!IsValid(MyBlackboard))
	{
		return;
	}

	for (AActor* ActorInArray : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(ActorInArray, PerceptionInfo);

		/*
		Note: We technically should Cast the actor who recieves the stimuli via PerceptionInfo.Target
		but because, in this case, we know that this is the Same actor as ActorInArray, we can just
		cast Actor in Array
		*/
		AAH_Character* CharacterThatSensed = Cast<AAH_Character>(ActorInArray);

		if (IsValid(CharacterThatSensed) && CharacterThatSensed->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			//Go through the perceived senses
			for (int i = 0; i < PerceptionInfo.LastSensedStimuli.Num(); i++)
			{
				switch (i)
				{
				case 0:
					MyBlackboard->SetValueAsBool(CanSeePlayerParameterName, PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());
					break;
				case 1:
					MyBlackboard->SetValueAsBool(IsReceivingDamageParameterName, PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());
					break;
				default:
					break;
				}

			}
		}
	}
}
