// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_LaunchPad.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;
class USoundCue;

UCLASS()
class ARKDE_ARTHEIST_API AAH_LaunchPad : public AActor
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LaunchPadComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* LaunchPadColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ActivationSignComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* DeActivationSignComponent;

protected:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launching")
	FVector LaunchDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* LaunchingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* LaunchpadInactiveSound;

public:
	//Public variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launching")
	bool bIsLaunchPadActive;
	
public:	
	// Sets default values for this actor's properties
	AAH_LaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void LaunchPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayLaunchingSound();

	void PlayLaunchpadInactiveSound();
public:	
	// Called every frame

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_ShowActivationStatusSign();

	virtual void Tick(float DeltaTime) override;


};
