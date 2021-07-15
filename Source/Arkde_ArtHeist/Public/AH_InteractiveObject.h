// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_InteractiveObject.generated.h"

class UBoxComponent; //Foward Declaration
class AAH_Character;

UCLASS()
class ARKDE_ARTHEIST_API AAH_InteractiveObject : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TheMainColliderComponent;

protected:
	//functions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision")
	void BP_CharacterEntersCollision();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision")
	void BP_CharacterLeavesCollision();

public:	
	// Sets default values for this actor's properties
	AAH_InteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//NOTE: I can only use this function if the RootComponent of the object is a collider

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void Action();

};
