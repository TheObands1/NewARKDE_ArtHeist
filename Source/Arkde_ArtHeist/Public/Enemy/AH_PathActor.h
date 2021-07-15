// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_PathActor.generated.h"


class UBillboardComponent;

UCLASS()
class ARKDE_ARTHEIST_API AAH_PathActor : public AActor
{
	GENERATED_BODY()
	
protected:
	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* PathBillboardComponent;

public:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path", meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

public:	
	// Sets default values for this actor's properties
	AAH_PathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
