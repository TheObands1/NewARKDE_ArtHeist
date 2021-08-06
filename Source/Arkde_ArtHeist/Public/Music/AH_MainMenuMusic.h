// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_MainMenuMusic.generated.h"


class UAudioComponent;
class UBillboardComponent;

UCLASS()
class ARKDE_ARTHEIST_API AAH_MainMenuMusic : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MusicAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* MusicBillboardComponent;
	
public:	
	// Sets default values for this actor's properties
	AAH_MainMenuMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
