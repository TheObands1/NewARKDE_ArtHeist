// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AH_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AAH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsValid(InputComponent))
	{
		InputComponent->BindAction("Pause", IE_Pressed, this, &AAH_PlayerController::PauseTheGame).bExecuteWhenPaused = true;
	}
}

void AAH_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	bIsGamePaused = false;
}

void AAH_PlayerController::PauseTheGame()
{
	if (bShouldThisPause)
	{
		if (!bIsGamePaused)
		{
			bIsGamePaused = true;
			UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);
			BP_PauseGame(bIsGamePaused);//Add PauseMenuWidget
			return;
		}

		if (bIsGamePaused)
		{
			bIsGamePaused = false;
			UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);
			BP_UnPauseGame(bIsGamePaused);//Delete PauseMenuWidget
			return;
		}
	}
}
