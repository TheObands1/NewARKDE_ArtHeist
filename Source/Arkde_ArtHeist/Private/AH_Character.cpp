// Fill out your copyright notice in the Description page of Project Settings.


#include "AH_Character.h"
#include "AH_InteractiveObject.h"
#include "Arkde_ArtHeist/Arkde_ArtHeist.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/AH_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AH_HealthComponent.h"
#include "Core/AH_GameMode.h"
#include "Core/AH_GameInstance.h"
#include "Core/AH_PlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/AH_BurningSymbol.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
//#include "Niagara/Public/NiagaraComponent.h"
//#include "Niagara/Public/NiagaraFunctionLibrary.h"


// Sets default values
AAH_Character::AAH_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	MeleeArmSocketName = "SCK_ArmMelee";
	MainMenuMapName = "MainMenu";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(),FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);

	MeleeArmDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeArmDetectorComponent"));
	MeleeArmDetectorComponent->SetupAttachment(GetMesh(), MeleeArmSocketName);
	MeleeArmDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeArmDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);

	WidgetBurningComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetBurningComponent"));
	WidgetBurningComponent->SetupAttachment(RootComponent);

	StepSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepSoundComponent"));
	StepSoundComponent->SetupAttachment(RootComponent);

	VoiceSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceSoundComponent"));
	VoiceSoundComponent->SetupAttachment(RootComponent);
	
	HealthComponent = CreateDefaultSubobject<UAH_HealthComponent>(TEXT("HealthComponent"));

	MeleeDamage = 10.0f;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultiplier = 1.0f;
	OriginalBurningCounter = 6;
	BurningCounter = 6;
	BurningDamage = 5.0f;
	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	UltimateFrequency = 0.5;
	UltimateWalkSpeed = 2000;
	UltimatePlayRate = 2;
	NormalPlayRate = 1;
	UltimateShootingFrequency = 0.1f;

	UltimateMeleeColliderHalfHeight = 350;
	UltimateMeleeColliderLocation = FVector(0, 330, 0);
	UltimateMeleeColliderRadius = 150;
	NormalMeleeColliderHalfHeight = 44;
	NormalMeleeColliderLocation = FVector(0, 0, 0);
	NormalMeleeColliderRadius = 15;
	
	UltimateMeleeArmColliderHalfHeight = 350;
	UltimateMeleeArmColliderRadius = 150;
	UltimateMeleeArmColliderLocation = FVector(330, 3, 3);
	NormalMeleeArmColliderHalfHeight = 45;
	NormalMeleeArmColliderLocation = FVector(0, 3, 2.4);
	NormalMeleeArmColliderRadius = 13;
	
	bUltimateWithTick = false;
	bIsGamePaused = false;

}

// Called when the game starts or when spawned
void AAH_Character::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
	CreateInitialWeapon();
	InitializeVariables();

}
// Called every frame
void AAH_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsUsingUltimate && bUltimateWithTick)
	{
		//RIFLE DOES NOT GO HERE ITS JUST FOR NOW
		if (CurrentUltimateType == EAH_UltimateType::UltimateType_Rifle)
		{
			UpdateUltimateDuration(DeltaTime, EAH_UltimateType::UltimateType_Rifle);
		}
		if (CurrentUltimateType == EAH_UltimateType::UltimateType_Melee)
		{
			UpdateUltimateDuration(DeltaTime, EAH_UltimateType::UltimateType_Melee);
		}
	}

}

FVector AAH_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirstPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}

	if (IsValid(TPSCameraComponent) && !bUseFirstPersonView)
	{
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AAH_Character::InitializeReferences()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<AAH_GameMode>(GetWorld()->GetAuthGameMode());
	GameInstanceReference = Cast<UAH_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(WidgetBurningComponent->GetUserWidgetObject()))
	{
		BurningSymbolReference= Cast<UAH_BurningSymbol>(WidgetBurningComponent->GetUserWidgetObject());
		if (IsValid(BurningSymbolReference))
		{
			DeactivateBurningSymbol();
		}
	}
}

void AAH_Character::InitializeVariables()
{
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_Character::MakeMeleeDamage);
	MeleeArmDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AAH_Character::MakeMeleeDamage);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AAH_Character::OnHealthChange);
	bCanUseWeapon = true;
	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MeleeDetectorComponent->SetCapsuleHalfHeight(NormalMeleeColliderHalfHeight);
	MeleeDetectorComponent->SetRelativeLocation(NormalMeleeColliderLocation);
	MeleeDetectorComponent->SetCapsuleRadius(NormalMeleeColliderRadius);

	MeleeArmDetectorComponent->SetCapsuleHalfHeight(NormalMeleeArmColliderHalfHeight);
	MeleeArmDetectorComponent->SetRelativeLocation(NormalMeleeArmColliderLocation);
	MeleeArmDetectorComponent->SetCapsuleRadius(NormalMeleeArmColliderRadius);

	NormalMeleeDamage = MeleeDamage;
}

void AAH_Character::MoveFoward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void AAH_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void AAH_Character::Jump()
{
	if (JumpCounter < MaxJumps)
	{
		JumpCounter += 1;
		PlayVoiceSound(JumpSound);
		Super::Jump();
	}
}

void AAH_Character::StopJumping()
{
	Super::StopJumping();
}

void AAH_Character::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

void AAH_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool AAH_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

bool AAH_Character::TryToAddHealth(float HealthToAdd)
{
	return HealthComponent->TryToAddHealth(HealthToAdd);
}

float AAH_Character::GetHealth()
{
	return HealthComponent->GetHealth();
}

float AAH_Character::GetMaxHealth()
{
	return HealthComponent->GetMaxHealth();
}

void AAH_Character::Burn()
{
	ActivateBurningSymbol();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Burning, this, &AAH_Character::DamageDueToBurn, 1.0f, true);
}

void AAH_Character::DamageDueToBurn()
{
	if (--BurningCounter <= 0)
	{
		BurningCounter = OriginalBurningCounter;
		DeactivateBurningSymbol();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Burning);
	}
	else
	{
		UGameplayStatics::ApplyDamage(this, BurningDamage, this->GetController(), this, MyDamageType);
	}
}

void AAH_Character::PlayStepSound()
{
	StepSoundComponent->Play();
}

void AAH_Character::Interact()
{
	if (InteractiveObject != nullptr)
	{
		InteractiveObject->Action();
	}
}

void AAH_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass)) //Verifies if the reference to the class already exists in a BP
	{
		//Spawns Weapon into the world, right where the player is
		CurrentWeapon = GetWorld()->SpawnActor<AAH_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());

		//If weapon is spwaned correctly and exists
		if (IsValid(CurrentWeapon))
		{	
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void AAH_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();

		if (bIsUsingUltimate && CurrentUltimateType == EAH_UltimateType::UltimateType_Rifle)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomaticShooting, CurrentWeapon, &AAH_Weapon::StartAction, UltimateShootingFrequency, true);
		}
	}
}

void AAH_Character::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShooting);
		}
	}
}

void AAH_Character::StartMelee()
{
	if (bIsDoingMelee && !bCanMakeCombos)
	{
		return;
	}

	if (bCanMakeCombos)
	{
		if (bIsDoingMelee)
		{
			if (bIsComboEnabled)
			{
				if (CurrentComboMultiplier < MaxComboMultiplier)
				{
					CurrentComboMultiplier++;
					SetComboState(false);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
	}

	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage, NormalPlayRate);
		PlayVoiceSound(MeleeSound);
		
		if (CurrentComboMultiplier <= 2)
		{
			MyAnimInstance->Montage_JumpToSection("Default", MeleeMontage);
		}
		if (CurrentComboMultiplier == 3)
		{
			MyAnimInstance->Montage_JumpToSection("Combo3", MeleeMontage);
		}
		if (CurrentComboMultiplier == 4)
		{
			MyAnimInstance->Montage_JumpToSection("Combo4", MeleeMontage);
		}
		
	}

	SetMeleeState(true);
}

void AAH_Character::StopMelee()
{

}

void AAH_Character::InitializeUltimateRilfeBehavior()
{
	CurrentUltimateType = EAH_UltimateType::UltimateType_Rifle;
	StartUltimate(EAH_UltimateType::UltimateType_Rifle);
}

void AAH_Character::InitializeUltimateMeleeBehavior()
{
	CurrentUltimateType = EAH_UltimateType::UltimateType_Melee;
	StartUltimate(EAH_UltimateType::UltimateType_Melee);
}

void AAH_Character::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void AAH_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor == this)
		{
			return;
		}

		AAH_Character* MeleeTarget = Cast<AAH_Character>(OtherActor);

		if (IsValid(MeleeTarget))
		{ 
			bool bIsPlayerAttackingEnemy = (GetCharacterType() == EAH_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == EAH_CharacterType::CharacterType_Enemy);
			bool bIsEnemyAttackingPlayer = (GetCharacterType() == EAH_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == EAH_CharacterType::CharacterType_Player);

			if (bIsPlayerAttackingEnemy || bIsEnemyAttackingPlayer)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, (MeleeDamage * CurrentComboMultiplier), SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			}
		}
		else
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, (MeleeDamage * CurrentComboMultiplier), SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}
	}
}

void AAH_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);

	if (CurrentComboMultiplier > 2 && CurrentComboMultiplier <= MaxComboMultiplier)
	{
		MeleeArmDetectorComponent->SetCollisionEnabled(NewCollisionState);
	}

}

void AAH_Character::SetComboState(bool NewState)
{
	bIsComboEnabled = NewState;
}

void AAH_Character::ResetCombo()
{
	SetComboState(false);
	CurrentComboMultiplier = 1.0f;
	MeleeArmDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //helps
}

void AAH_Character::StartUltimate(EAH_UltimateType UltimateType)
{
	if (bCanUseUltimate && !bIsUsingUltimate)
	{
		CurrentUltimateDuration = MaxUltimateDuration;
		bCanUseUltimate = false;
		BP_StartUltimate(UltimateType);
		TimerDelegate_BeginUltimateBehaviour.BindUFunction(this, FName("BeginUltimateBehaviour"), UltimateType);

		if (UltimateType == EAH_UltimateType::UltimateType_Rifle)
		{
			PlayVoiceSound(UltimateRifleSound);

			if (IsValid(MyAnimInstance) && IsValid(UltimateRifleMontage))
			{
				GetCharacterMovement()->MaxWalkSpeed = 0;
				const float StartUltimateRifleMontageDuration = MyAnimInstance->Montage_Play(UltimateRifleMontage);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehaviour, TimerDelegate_BeginUltimateBehaviour, StartUltimateRifleMontageDuration, false);
			}
			else
			{
				BeginUltimateBehaviour(UltimateType);
			}

		}

		if (UltimateType == EAH_UltimateType::UltimateType_Melee)
		{
			PlayVoiceSound(UltimateMeleeSound);

			if (IsValid(MyAnimInstance) && IsValid(UltimateMeleeMontage))
			{
				GetCharacterMovement()->MaxWalkSpeed = 0;
				MeleeDamage = MeleeDamage + 10;
				const float StartUltimateMeleeMontageDuration = MyAnimInstance->Montage_Play(UltimateMeleeMontage);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehaviour, TimerDelegate_BeginUltimateBehaviour, StartUltimateMeleeMontageDuration, false);
			}
			else
			{
				BeginUltimateBehaviour(UltimateType);
			}
		}
	}
}

void AAH_Character::StopUltimate()
{
}

void AAH_Character::PlayVoiceSound(USoundCue* VoiceSound)
{
	if (!IsValid(VoiceSound))
	{
		return;
	}

	VoiceSoundComponent->SetSound(VoiceSound);
	VoiceSoundComponent->Play();
}

void AAH_Character::GainUltimateXP(float XPGained)
{
	if (bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp((CurrentUltimateXP + XPGained), 0.0F, MaxUltimateXP);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateXP, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
		OnUltimateStatusDelegate.Broadcast(true);
	}

	BP_GainUltimateXP(XPGained);
}

void AAH_Character::BeginUltimateBehaviour(EAH_UltimateType UltimateType)
{
	bIsUsingUltimate = true;

	if (UltimateType == EAH_UltimateType::UltimateType_Rifle)
	{
		GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
		NormalPlayRate = UltimatePlayRate;
	}

	if (UltimateType == EAH_UltimateType::UltimateType_Melee)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		//Put New Size To colliders
		MeleeDetectorComponent->SetCapsuleHalfHeight(UltimateMeleeColliderHalfHeight);
		MeleeDetectorComponent->SetCapsuleRadius(UltimateMeleeColliderRadius);
		MeleeDetectorComponent->SetRelativeLocation(UltimateMeleeColliderLocation);

		MeleeArmDetectorComponent->SetCapsuleHalfHeight(UltimateMeleeArmColliderHalfHeight);
		MeleeArmDetectorComponent->SetCapsuleRadius(UltimateMeleeArmColliderRadius);
		MeleeArmDetectorComponent->SetRelativeLocation(UltimateMeleeArmColliderLocation);
	}

	if (!bUltimateWithTick)
	{
		TimerDelegate_UltimateDuration.BindUFunction(this, FName("UpdateUltimateDurationWithTimer"), UltimateType);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ultimate, TimerDelegate_UltimateDuration, UltimateFrequency, true);
	}
}

void AAH_Character::UpdateUltimateDuration(float Value, EAH_UltimateType UltimateType)
{
	CurrentUltimateDuration = FMath::Clamp((CurrentUltimateDuration - Value), 0.0f, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value, UltimateType);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateDuration, MaxUltimateDuration);

	if (CurrentUltimateDuration == 0 && bIsUsingUltimate)
	{
		bIsUsingUltimate = false;
		CurrentUltimateXP = 0.0f;
		OnUltimateStatusDelegate.Broadcast(false);

		if (UltimateType == EAH_UltimateType::UltimateType_Rifle)
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
			NormalPlayRate = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShooting);
		}

		if (UltimateType == EAH_UltimateType::UltimateType_Melee)
		{
			//Pput colliders normally
			MeleeDetectorComponent->SetCapsuleHalfHeight(NormalMeleeColliderHalfHeight);
			MeleeDetectorComponent->SetRelativeLocation(NormalMeleeColliderLocation);
			MeleeDetectorComponent->SetCapsuleRadius(NormalMeleeColliderRadius);

			MeleeArmDetectorComponent->SetCapsuleHalfHeight(NormalMeleeArmColliderHalfHeight);
			MeleeArmDetectorComponent->SetRelativeLocation(NormalMeleeArmColliderLocation);
			MeleeArmDetectorComponent->SetCapsuleRadius(NormalMeleeArmColliderRadius);

			MeleeDamage = NormalMeleeDamage;
		}

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ultimate);
		}

		BP_StopUltimate(UltimateType);
	}
}

void AAH_Character::UpdateUltimateDurationWithTimer(EAH_UltimateType UltimateType)
{
	UpdateUltimateDuration(UltimateFrequency, UltimateType);
}

void AAH_Character::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}
}

void AAH_Character::ControlGamePausedState()
{
	/*
	if (!bIsGamePaused)
	{
		bIsGamePaused = true;
		BP_PauseGame(bIsGamePaused);//Add PauseMenuWidget
		return;
	}

	if (bIsGamePaused)
	{
		bIsGamePaused = false;
		BP_UnPauseGame(bIsGamePaused);//Delete PauseMenuWidget
		return;
	}
	*/
}


void AAH_Character::OnHealthChange(UAH_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!HealthComponent->IsDead())
	{
		PlayVoiceSound(HurtSound);
	}

	if (HealthComponent->IsDead())
	{
		PlayVoiceSound(DeathSound);
		if (GetCharacterType() == EAH_CharacterType::CharacterType_Player)
		{
			if (IsValid(GameModeReference))
			{
				GameModeReference->GameOver(this);
			}
		}
	}
}

void AAH_Character::ActivateBurningSymbol()
{
	if (IsValid(BurningSymbolReference))
	{
		BurningSymbolReference->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAH_Character::DeactivateBurningSymbol()
{
	if (IsValid(BurningSymbolReference))
	{
		BurningSymbolReference->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called to bind functionality to input
void AAH_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFoward", this, &AAH_Character::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAH_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AAH_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed ,this, &AAH_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAH_Character::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAH_Character::Interact);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &AAH_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &AAH_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AAH_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &AAH_Character::StopMelee);

	PlayerInputComponent->BindAction("UltimateRifle", IE_Pressed, this, &AAH_Character::InitializeUltimateRilfeBehavior);
	PlayerInputComponent->BindAction("UltimateRifle", IE_Released, this, &AAH_Character::StopUltimate);

	PlayerInputComponent->BindAction("UltimateMelee", IE_Pressed, this, &AAH_Character::InitializeUltimateMeleeBehavior);
	PlayerInputComponent->BindAction("UltimateMelee", IE_Released, this, &AAH_Character::StopUltimate);

	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &AAH_Character::GoToMainMenu);

	//PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AAH_Character::ControlGamePausedState).bExecuteWhenPaused = true;


}

