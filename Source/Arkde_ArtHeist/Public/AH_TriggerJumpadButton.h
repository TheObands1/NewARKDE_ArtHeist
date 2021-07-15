// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_TriggerJumpadButton.generated.h"

class UBoxComponent;
class AAH_LaunchPad;
class AAH_Character;
class UStaticMeshComponent;

UCLASS()

class ARKDE_ARTHEIST_API AAH_TriggerJumpadButton : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ButtonColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ButtonComponent;

	UPROPERTY(EditAnywhere, Category = "LaunchingPad")
	AAH_LaunchPad* LaunchingPad;
	
public:	
	// Sets default values for this actor's properties
	AAH_TriggerJumpadButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Used to determine if this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//NOTE: I can only use this function if the RootComponent of the object is a collider


};
