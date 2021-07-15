// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AH_PathActor.h"
#include "Components/BillboardComponent.h"

// Sets default values
AAH_PathActor::AAH_PathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PathBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathBillboardComponent"));
	RootComponent = PathBillboardComponent;

}

// Called when the game starts or when spawned
void AAH_PathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_PathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

