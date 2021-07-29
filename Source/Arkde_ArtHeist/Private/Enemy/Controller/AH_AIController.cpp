// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/AH_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enemy/AH_Enemy.h"
#include "AH_Character.h"

AAH_AIController::AAH_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	bWasBehaviorTreeLoadedSuccessfully = false;
	LoopPathParameterName = "bLoopPath";
	DirectionIndexParameterName = "DirectionIndex";
	WaitingTimeOnPathPointParameterName = "WaitingTimeOnPathPoint";
	CanSeePlayerParameterName = "bCanSeePlayer";
	InvestigatingParameterName = "bIsInvestigating";
	TargetLocationParameterName = "TargetLocation";
	bIsReceivingDamage = false;
}

void AAH_AIController::DeactivateAIPerception()
{
	AIPerceptionComponent->Deactivate();
}

void AAH_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(EnemyBehaviorTree))
	{
		RunBehaviorTree(EnemyBehaviorTree);
		bWasBehaviorTreeLoadedSuccessfully = true;
	}

	MyEnemy = Cast<AAH_Enemy>(K2_GetPawn());

	if (IsValid(MyEnemy) && bWasBehaviorTreeLoadedSuccessfully)
	{
		MyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

		if (IsValid(MyBlackboard))
		{
			MyBlackboard->SetValueAsBool(LoopPathParameterName, MyEnemy->GetLoopPath());
			MyBlackboard->SetValueAsInt(DirectionIndexParameterName, MyEnemy->GetDirectionIndex());
			MyBlackboard->SetValueAsFloat(WaitingTimeOnPathPointParameterName, MyEnemy->GetWaitingTimeOnPathPoint());
		}
	}


	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAH_AIController::UpdateSenses);

}

void AAH_AIController::UpdateSenses(const TArray<AActor*>& UpdatedActors)
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
					if (IsValid(MyEnemy))
					{
						MyEnemy->SetIsAlert(PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());
					}
					break;
				case 1:
					MyBlackboard->SetValueAsBool(InvestigatingParameterName, bIsReceivingDamage);

					if (bIsReceivingDamage)
					{
						MyBlackboard->SetValueAsVector(TargetLocationParameterName, PerceptionInfo.LastSensedStimuli[i].StimulusLocation);
					}
					break;
				default:
					break;
				}

			}
		}
	}
}
