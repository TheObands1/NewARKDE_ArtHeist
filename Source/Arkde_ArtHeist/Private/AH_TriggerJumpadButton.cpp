// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_TriggerJumpadButton.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AH_Character.h"
#include "AH_LaunchPad.h"

// Sets default values
AAH_TriggerJumpadButton::AAH_TriggerJumpadButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonColliderComponent"));
	ButtonColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ButtonColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ButtonColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ButtonColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ButtonColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = ButtonColliderComponent;

	ButtonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonComponent"));
	ButtonComponent->SetupAttachment(ButtonColliderComponent);

}

// Called when the game starts or when spawned
void AAH_TriggerJumpadButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_TriggerJumpadButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_TriggerJumpadButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Verifies is actor is valid
	if (IsValid(OtherActor))
	{
		//Asigns "OverlappedCharacter" a type. However, because of how we are using Cast here, if
		//the actor is something other than a character, then "OverlappedCharacter"'s value will be null
		AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
		//Verifies if actor is a character
		if (IsValid(OverlappedCharacter))
		{
			//If the actor is a character, change value of boolean in LaunchPad
			if (IsValid(LaunchingPad)) //Verifies launchpad existence
			{
				if (LaunchingPad->bIsLaunchPadActive)
				{
					LaunchingPad->bIsLaunchPadActive = false;
				}
				else
				{
					LaunchingPad->bIsLaunchPadActive = true;
				}
			}
		}
	}
}

