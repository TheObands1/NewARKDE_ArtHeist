// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_LaunchPadButton.h"
#include "AH_LaunchPad.h"

AAH_LaunchPadButton::AAH_LaunchPadButton()
{
	ButtonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMeshComponent"));
	ButtonMeshComponent->SetupAttachment(RootComponent);
}

void AAH_LaunchPadButton::Action()
{
	
	if(IsValid(LaunchingPad))
	{ 
		
		bIsTheButtonPressed = LaunchingPad->bIsLaunchPadActive;
		bIsTheButtonPressed = !bIsTheButtonPressed;
		LaunchingPad->bIsLaunchPadActive = bIsTheButtonPressed;
		BP_ButtonPressed();
		
	}
}
