// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_Door.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ARKDE_ARTHEIST_API AAH_Door : public AActor
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* KeyZoneColliderComponent;

protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Door")
	FName DoorTag;

protected:
	//FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision")
	void BP_PlayerNeedsKey();
	
public:	
	// Sets default values for this actor's properties
	//Constructor
	AAH_Door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Every function that is going to unite/bind to a Delegate needs a UFUNCTION Macro, even if it is empty
	UFUNCTION()
	void CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Door")
	void BP_OpenDoor();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();

	
};
