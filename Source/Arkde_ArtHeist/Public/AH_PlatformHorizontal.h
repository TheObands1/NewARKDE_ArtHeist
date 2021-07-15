// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_PlatformHorizontal.generated.h"

class UStaticMeshComponent; //Foward Declaration

UCLASS()
class ARKDE_ARTHEIST_API AAH_PlatformHorizontal : public AActor
{
	GENERATED_BODY()

protected:
	//Sección de componentes
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* PlatformMeshComponent;


protected:
	//Sección de variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		bool bIsGoingRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float MinRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
		float MaxRight;


	
public:	
	// Sets default values for this actor's properties
	AAH_PlatformHorizontal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Move();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
