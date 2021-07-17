// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AH_HUDObjective.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API UAH_HUDObjective : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FName ObjectiveName;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	bool bIsObjectiveCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	FLinearColor CompletedColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	FLinearColor CurrentColor;
	
public:
	void CompleteObjective();
};
