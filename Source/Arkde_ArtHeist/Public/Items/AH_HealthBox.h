// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/AH_Item.h"
#include "AH_HealthBox.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_ARTHEIST_API AAH_HealthBox : public AAH_Item
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthBoxMeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthValue")
	float HealthValue;

public:
	// Sets default values for this actor's properties
	AAH_HealthBox();
public:
	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthValue() const { return HealthValue; };
protected:
	//Functions
	virtual void Pickup(AAH_Character* PickupCharacter) override;

};
