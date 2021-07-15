// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AH_AICivilianController.generated.h"

/**
 * 
 */

class UAIPerceptionComponent;
class AAH_Civilian;
class UBlackboardComponent;
class UBehaviorTree;


UCLASS()
class ARKDE_ARTHEIST_API AAH_AICivilianController : public AAIController
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComponent;

protected:
	//Variables

	bool bWasBehaviorTreeLoadedSuccessfully;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Civilian Controller")
	FName CanSeePlayerParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Civilian Controller")
	FName IsReceivingDamageParameterName;

	UPROPERTY(BlueprintReadOnly, Category = "Civilian Controller")
	AAH_Civilian* MyCivilian;

	UPROPERTY(BlueprintReadOnly, Category = "Civilian Controller")
	UBlackboardComponent* MyBlackboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	UBehaviorTree* CivilianBehaviorTree;

public:
	// Constructor
	AAH_AICivilianController();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSenses(const TArray<AActor*>& UpdatedActors);
	
};
