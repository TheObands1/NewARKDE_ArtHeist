// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_SpectatingCamera.generated.h"

class UStaticMeshComponent;

UENUM(Blueprintable)
enum class EAH_SpectatingCameraType :uint8
{
	CameraType_None     UMETA(DisplayName = "None"),
	CameraType_Victory  UMETA(DisplayName = "Victory"),
	CameraType_GameOver UMETA(DisplayName = "GameOver")
};

UCLASS()
class ARKDE_ARTHEIST_API AAH_SpectatingCamera : public AActor
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpectatingCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
	EAH_SpectatingCameraType CameraType;
	
public:	
	// Sets default values for this actor's properties
	AAH_SpectatingCamera();

	EAH_SpectatingCameraType GetCameraType() {return CameraType;};

};
