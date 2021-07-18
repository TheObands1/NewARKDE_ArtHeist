// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AH_GameMode.h"
#include "AH_Character.h"
#include "AH_SpectatingCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAH_GameMode::AAH_GameMode()
{
	MainMenuMapName = "MainMenu";
}

void AAH_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();
}

void AAH_GameMode::SetupSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAH_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			AAH_SpectatingCamera* SpectatingCamera = Cast<AAH_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case EAH_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case EAH_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}

			}
		}
	}
}

void AAH_GameMode::MoveCameraToSpectatingPoint(AAH_Character* Character, AAH_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(Character) || !IsValid(SpectatingCamera))
	{
		return;
	}
	
	if (IsValid(Character) && IsValid(SpectatingCamera))
	{
		AController* CharacterController = Character->GetController();
		if (IsValid(CharacterController))
		{
			APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
			if (IsValid(PlayerController))
			{
				PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}

}

void AAH_GameMode::Victory(AAH_Character* Character)
{
	OnVictoryDelegate.Broadcast();
	Character->DisableInput(nullptr);
	MoveCameraToSpectatingPoint(Character, VictoryCamera);
	BP_Victory(Character);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AAH_GameMode::BackToMainMenu, 3.0f, false);
	
}


void AAH_GameMode::GameOver(AAH_Character* Character)
{
	OnGameOverDelegate.Broadcast();


	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Character->HasToDestroy())
	{
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(5.0);
	}
	else
	{
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(Character, GameOverCamera);
	}
	BP_GameOver(Character);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AAH_GameMode::BackToMainMenu, 3.0f, false);
}

void AAH_GameMode::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void AAH_GameMode::AddKeyToCharacter(AAH_Character* KeyOwner, FName KeyTag)
{
	if (IsValid(KeyOwner))
	{
		OnKeyAddedDelegate.Broadcast(KeyTag);
		KeyOwner->AddKey(KeyTag);
	}
}
