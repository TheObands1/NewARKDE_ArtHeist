// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AH_Character.h"

// Sets default values
AAH_Door::AAH_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent->SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(CustomRootComponent);

	KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
	KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
	KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	OpenAngle = -90.0f;
	DoorTag = "KeyA";

}

// Called when the game starts or when spawned
void AAH_Door::BeginPlay()
{
	Super::BeginPlay();
	KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_Door::CheckKeyFromPlayer);
	//El adicionar funciones va en el BeginPlay, No constructor, porque primero debe crear todo el componente y luego en ejecución saber que se va a añadir
}

// Called every frame
void AAH_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAH_Door::OpenDoor()
{
	bIsOpen = true;
	BP_OpenDoor(); //This function handles the animation for the opening of the door
}

void AAH_Door::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Verifies if door is already open. If it is, then it doesn't do the collision check
	if (bIsOpen)
	{
		return;
	}
	//Verifies is actor is valid
	if (IsValid(OtherActor))
	{
		//Asigns "OverlappedCharacter" a type. However, because of how we are using Cast here, if
		//the actor is something other than a character, then "OverlappedCharacter"'s value will be null
		AAH_Character* OverlappedCharacter = Cast<AAH_Character>(OtherActor);
		//Verifies if actor is a character
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			//Veryfies if character has the key of Doortag
			if (OverlappedCharacter->HasKey(DoorTag))
			{
				OpenDoor();
			}
			else
			{
				BP_PlayerNeedsKey();
			}
			
		}
	}
}