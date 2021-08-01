// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AH_KeySpawner.h"

// Sets default values
AAH_KeySpawner::AAH_KeySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAH_KeySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_KeySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

