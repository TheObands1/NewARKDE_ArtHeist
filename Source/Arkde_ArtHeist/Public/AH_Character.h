// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AH_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateUpdateSignature, float, CurrentUltimateXP, float, MaxUltimateXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateStatusSignature, bool, bIsAvailable);

class UCameraComponent;
class USpringArmComponent;
class AAH_InteractiveObject;
class AAH_Weapon;
class UAnimMontage;
class UAnimInstance;
class UAH_HealthComponent;
class USphereComponent;
class UAH_GameInstance;
class AAH_GameMode;
class UWidgetComponent;
class UAH_BurningSymbol;
class UAudioComponent;
//class UNiagaraSystem;

UENUM(Blueprintable)
enum class EAH_CharacterType :uint8
{
	CharacterType_Player     UMETA(DisplayName = "Player"),
	CharacterType_Enemy  UMETA(DisplayName = "Enemy"),
};

UENUM(Blueprintable)
enum class EAH_UltimateType :uint8
{
	UltimateType_Rifle     UMETA(DisplayName = "Ultimate Automatic Rifle"),
	UltimateType_Melee  UMETA(DisplayName = "Ultimate Melee Attacks"),
};

UCLASS()
class ARKDE_ARTHEIST_API AAH_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeArmDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAH_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetBurningComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* StepSoundComponent;

protected:
	//Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	bool bUseFirstPersonView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	bool bIsLookInversion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
	bool bHasToDestroy;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsDoingMelee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	bool bCanMakeCombos;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsComboEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	bool bUltimateWithTick;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bIsUsingUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bCanUseWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Pause Menu")
	bool bIsGamePaused;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jumping")
	int JumpCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jumping")
	int MaxJumps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Burning State")
	int OriginalBurningCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Burning State")
	int BurningCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Burning State")
	float BurningDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Melee")
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Melee")
	float NormalMeleeDamage;

	//meta and EditCondition here makes the MaxComboMultiplier editable only if the character can make combos
	//ClampMin states the minimum value the variable can have
	//UIMin states the minimum value one can assign to the variable in blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float CurrentComboMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	float CurrentUltimateXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time")
	float CurrentUltimateDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float UltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimatePlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float UltimateShootingFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float UltimateMeleeColliderHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float UltimateMeleeArmColliderHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float UltimateMeleeColliderRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float UltimateMeleeArmColliderRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalMeleeColliderHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalMeleeArmColliderHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalMeleeColliderRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalMeleeArmColliderRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	FVector UltimateMeleeColliderLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	FVector UltimateMeleeArmColliderLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	FVector NormalMeleeColliderLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
	FVector NormalMeleeArmColliderLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeArmSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu")
	FName  MainMenuMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<FName> DoorKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AAH_Weapon> InitialWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> MyDamageType;

	//Because it should be assigned in-game, it won't have an EditAnywhere property
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AAH_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateRifleMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateMeleeMontage;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UAH_BurningSymbol* BurningSymbolReference;
	
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* Ultimate2Effect1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* Ultimate2Effect2;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAH_CharacterType CharacterType;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	EAH_UltimateType CurrentUltimateType;

	UAH_GameInstance* GameInstanceReference;

	UAnimInstance* MyAnimInstance;

	AAH_GameMode* GameModeReference;

	APlayerController* PlayerControllerReference;

	FTimerHandle TimerHandle_Burning;

	FTimerHandle TimerHandle_Ultimate;

	FTimerHandle TimerHandle_AutomaticShooting;

	FTimerHandle TimerHandle_BeginUltimateBehaviour;

	FTimerDelegate TimerDelegate_BeginUltimateBehaviour;

//	FTimerDelegate TimerDelegate_BeginUltimateMeleeBehaviour;

	FTimerDelegate TimerDelegate_UltimateDuration;

public:
	//Public Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactables")
	AAH_InteractiveObject* InteractiveObject;

	UPROPERTY(BlueprintAssignable)
	FOnUltimateUpdateSignature OnUltimateUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUltimateStatusSignature OnUltimateStatusDelegate;

public:
	// Sets default values for this character's properties
	AAH_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Functions:

	void InitializeReferences();

	void InitializeVariables();

	void MoveFoward(float value);

	void MoveRight(float value);

	virtual void Jump() override;

	virtual void StopJumping() override;

	virtual void Interact();

	virtual void CreateInitialWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void StartWeaponAction();

	virtual void StopWeaponAction();

	UFUNCTION(BlueprintCallable)
	virtual void StartMelee();

	virtual void StopMelee();

	virtual void InitializeUltimateRilfeBehavior();

	virtual void InitializeUltimateMeleeBehavior();

	virtual void StartUltimate(EAH_UltimateType UltimateType);

	virtual void StopUltimate();

	UFUNCTION()
	virtual void BeginUltimateBehaviour(EAH_UltimateType UltimateType);

	void GoToMainMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pause Game")
	void BP_PauseGame(bool PausedState);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pause Game")
	void BP_UnPauseGame(bool PausedState);
	
protected:
	//Functions with delegates

	//Because this is a function that will be called via OnComponentBeginOverlap, and this function is a delegate
	//it needs to have a Macro. Also, it needs to have all the parameters of the delegate itself.
	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void MakeMeleeArmDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Burning State")
	void ActivateBurningSymbol();

	UFUNCTION(BlueprintCallable, Category = "Burning State")
	void DeactivateBurningSymbol();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	UFUNCTION(BlueprintCallable)
	void SetComboState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP(float XPGained);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate(EAH_UltimateType UltimateType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate(EAH_UltimateType UltimateType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value, EAH_UltimateType UltimateType);

	UFUNCTION(BlueprintCallable)
	void SetIsGamePaused(bool NewValue) { bIsGamePaused = NewValue; };

	UFUNCTION(BlueprintCallable)
	void ControlGamePausedState();

	void AddKey(FName NewKey);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	void Burn();

	void DamageDueToBurn();

	void PlayStepSound();

	virtual void UpdateUltimateDuration(float Value, EAH_UltimateType UltimateType);

	UFUNCTION()
	virtual void UpdateUltimateDurationWithTimer(EAH_UltimateType UltimateType);

	bool HasKey(FName KeyTag);

	bool HasToDestroy(){ return bHasToDestroy; };

	UFUNCTION(BlueprintCallable)
	bool TryToAddHealth(float HealthToAdd);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsGamePaused() { return bIsGamePaused; };

	float GetHealth();

	float GetMaxHealth();

	EAH_CharacterType GetCharacterType() { return CharacterType; };

	UAH_HealthComponent* GetHealthComponent() { return HealthComponent; };

	//EAH_UltimateType GetCurrentUltimate() {}

};
