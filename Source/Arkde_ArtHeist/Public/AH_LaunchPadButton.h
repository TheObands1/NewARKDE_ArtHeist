// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AH_InteractiveObject.h"
#include "AH_LaunchPadButton.generated.h"

class UStaticMeshComponent;
class AAH_LaunchPad;
/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API AAH_LaunchPadButton : public AAH_InteractiveObject
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ButtonMeshComponent;

protected:
	//BP_Functions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ButtonLaunchPad")
	void BP_ButtonPressed();

public:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonLaunchPad")
	AAH_LaunchPad* LaunchingPad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonLaunchPad")
	bool bIsTheButtonPressed;


public:
	// Sets default values for this actor's properties
	AAH_LaunchPadButton();

protected:
	//Functions
	virtual void Action() override;
};
