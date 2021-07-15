// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AH_Item.h"
#include "Components/SphereComponent.h"
#include "AH_Character.h"

// Sets default values
AAH_Item::AAH_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MainColliderComponent;

}

// Called when the game starts or when spawned
void AAH_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAH_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_Item::NotifyActorBeginOverlap(AActor* OtherActor)  
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
			//If the actor is a character, Pickup function is called.
			Pickup(OverlappedCharacter);
		}
	}
}

void AAH_Item::Pickup(AAH_Character* PickupCharacter)
{
	BP_Pickup(PickupCharacter);
}

