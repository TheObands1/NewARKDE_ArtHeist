// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AH_GameMode.h"
#include "AH_Character.h"
#include "AH_SpectatingCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy/AH_Enemy.h"

AAH_GameMode::AAH_GameMode()
{
	MainMenuMapName = "MainMenu";
	TimeToGoBackToMenuAfterVictory = 8.0f;
	TimeToGoBackToMenuAfterGameOver = 8.0f;
	bIsInAlertMode = false;
}

void AAH_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();
	SetupEnemiesInLevel();


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

void AAH_GameMode::PlayMusic(USoundCue* SoundCueToPlay)
{
	if (!IsValid(SoundCueToPlay))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), SoundCueToPlay);


}

void AAH_GameMode::SetupEnemiesInLevel()
{
	TArray<AActor*> PossibleEnemyActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAH_Enemy::StaticClass(), PossibleEnemyActors);

	for (AActor* PossibleEnemy : PossibleEnemyActors)
	{
		if (!IsValid(PossibleEnemy))
		{
			continue;
		}

		AAH_Enemy* NewEnemyToAdd = Cast<AAH_Enemy>(PossibleEnemy);
		if (IsValid(NewEnemyToAdd))
		{
			LevelEnemies.AddUnique(NewEnemyToAdd);
		}
	}
}

void AAH_GameMode::Victory(AAH_Character* Character)
{
	OnVictoryDelegate.Broadcast();
	Character->DisableInput(nullptr);
	MoveCameraToSpectatingPoint(Character, VictoryCamera);
	BP_Victory(Character);
	PlayMusic(VictoryMusic);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AAH_GameMode::BackToMainMenu, TimeToGoBackToMenuAfterVictory, false);
	
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
	PlayMusic(GameOverMusic);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AAH_GameMode::BackToMainMenu, TimeToGoBackToMenuAfterGameOver, false);
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

void AAH_GameMode::CheckAlertMode()
{
	bool bIsEnemyInAlertMode = false;
	for (AAH_Enemy* Enemy : LevelEnemies)
	{
		if (!IsValid(Enemy))
		{
			continue;
		}

		if (Enemy->GetIsAlert())
		{
			bIsEnemyInAlertMode = true;
			break;
		}

	}

	if (bIsInAlertMode != bIsEnemyInAlertMode)
	{
		bIsInAlertMode = bIsEnemyInAlertMode;
		OnAlertModeChangeDelegate.Broadcast(bIsInAlertMode);
	}
}
