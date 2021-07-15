// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/AH_Item.h"
#include "AH_DoorKey.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API AAH_DoorKey : public AAH_Item
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	float XPValue;

public:
	// Sets default values for this actor's properties
	AAH_DoorKey();
public:
	UFUNCTION(BlueprintCallable, Category = "Getter")
		FName GetKeyTag() const { return KeyTag; };
protected:
	//Functions
	virtual void Pickup(AAH_Character* PickupCharacter) override;
	
};
