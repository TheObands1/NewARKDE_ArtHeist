// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_KeySpawner.generated.h"

class UBillboardComponent;
class AAH_DoorKey;

UCLASS()
class ARKDE_ARTHEIST_API AAH_KeySpawner : public AActor
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerBillboardComponent;

protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AAH_DoorKey> DoorKeyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawner")
	FName KeyTagOfSpawnedKey;
	
public:	
	// Sets default values for this actor's properties
	AAH_KeySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnKey();

	FVector GetSpawnPoint();
};
