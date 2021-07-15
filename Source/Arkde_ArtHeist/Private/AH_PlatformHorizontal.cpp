// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_PlatformHorizontal.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAH_PlatformHorizontal::AAH_PlatformHorizontal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;


	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMeshComponent->SetupAttachment(CustomRootComponent);

	bIsGoingRight = true;
	Speed = 10.0f;
	MinRight = 0.0f;
	MaxRight = 50.0f;

}

// Called when the game starts or when spawned
void AAH_PlatformHorizontal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_PlatformHorizontal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();

}


void AAH_PlatformHorizontal::Move()
{
	FVector rightDirection = UKismetMathLibrary:: GetRightVector(GetActorRotation());

	if (bIsGoingRight)
	{
		rightDirection = rightDirection;

	}
	else
	{
		rightDirection = rightDirection * -1;
	}

	FVector CurrentVelocity = rightDirection * Speed;
	FVector NewPosition = GetActorLocation() + CurrentVelocity;
	SetActorLocation(NewPosition);

	if ((bIsGoingRight && GetActorLocation().Y >= MaxRight) || (!bIsGoingRight && GetActorLocation().Y <= MinRight))
	{
		bIsGoingRight = !bIsGoingRight;
	}
}

