// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_MainMenuMusic.generated.h"

UCLASS()
class ARKDE_ARTHEIST_API AAH_MainMenuMusic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAH_MainMenuMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
