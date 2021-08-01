// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AH_Character.h"
#include "Components/WidgetComponent.h"

// Sets default values
AAH_LaunchPad::AAH_LaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	LaunchPadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadComponent"));
	LaunchPadComponent->SetupAttachment(CustomRootComponent);

	LaunchPadColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchPadColliderComponent"));
	LaunchPadColliderComponent->SetupAttachment(CustomRootComponent);
	LaunchPadColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LaunchPadColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaunchPadColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ActivationSignComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActivationSignComponent"));
	ActivationSignComponent->SetupAttachment(RootComponent);

	DeActivationSignComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DeActivationSignComponent"));
	DeActivationSignComponent->SetupAttachment(RootComponent);

	LaunchDirection = FVector(0, 0, 1500);
	bIsLaunchPadActive = false;
}

// Called when the game starts or when spawned
void AAH_LaunchPad::BeginPlay()
{
	Super::BeginPlay();
	LaunchPadColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_LaunchPad::LaunchPlayer);
	
}

void AAH_LaunchPad::LaunchPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BP_ShowActivationStatusSign();
	if (bIsLaunchPadActive)
	{
		if (IsValid(OtherActor))
		{
			//Asigns "OverlappedCharacter" a type. However, because of how we are using Cast here, if
			//the actor is something other than a character, then "OverlappedCharacter"'s value will be null
			AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
			//Verifies if actor is a character
			if (IsValid(OverlappedCharacter))
			{
				OverlappedCharacter->LaunchCharacter(LaunchDirection, false, false);
			}
		}
	}
}

// Called every frame
void AAH_LaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

