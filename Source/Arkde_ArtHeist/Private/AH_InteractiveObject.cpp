// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "AH_Character.h"

// Sets default values
AAH_InteractiveObject::AAH_InteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TheMainColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TheMainColliderComponent"));
	TheMainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TheMainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TheMainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	TheMainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	TheMainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = TheMainColliderComponent;

}

// Called when the game starts or when spawned
void AAH_InteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAH_InteractiveObject::Action()
{
}

// Called every frame
void AAH_InteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_InteractiveObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Verifies is actor is valid
	if (IsValid(OtherActor))
	{
		//Asigns "OverlappedCharacter" a type. However, because of how we are using Cast here, if
		//the actor is something other than a character, then "OverlappedCharacter"'s value will be null
		AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
		//Verifies if actor is a character
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			//If actor is a player character, send reference to Character and show interact button
			OverlappedCharacter->InteractiveObject = this;
			BP_CharacterEntersCollision();
		}
	}

}
void AAH_InteractiveObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	//Verifies is actor is valid
	if (IsValid(OtherActor))
	{
		//Asigns "OverlappedCharacter" a type. However, because of how we are using Cast here, if
		//the actor is something other than a character, then "OverlappedCharacter"'s value will be null
		AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
		//Verifies if actor is a character
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			OverlappedCharacter->InteractiveObject = nullptr;
			BP_CharacterLeavesCollision();
		}
	}

}

