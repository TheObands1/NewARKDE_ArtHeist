// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_Item.generated.h"

class USphereComponent; //Foward Declaration
class AAH_Character;
class AAH_GameMode;
class USoundCue;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Item : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* MainColliderComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	AAH_GameMode* GameModeReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* PickUpSound;

	
public:	
	// Sets default values for this actor's properties
	AAH_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Pickup(AAH_Character* PickupCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
	void BP_Pickup(AAH_Character* PickupCharacter);

	void PlayPickUpSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Used to determine if this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//NOTE: I can only use this function if the RootComponent of the object is a collider

};
