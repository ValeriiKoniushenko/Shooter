// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SBasePlayerController.h"
#include "GAS/SMainCharacterAttributeSet.h"
#include "Animation/AnimMontage.h"
#include "Camera/SMainCameraComponent.h"
#include "MovementComponent/MainCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASBaseCharacter::ASBaseCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UMainCharacterMovementComponent>(
		CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeSet = CreateDefaultSubobject<USMainCharacterAttributeSet>(TEXT("AttributeSet"));
	check(AttributeSet);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	check(AbilitySystemComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->SetupAttachment(GetMesh(), "HeadSocket");

	CameraComponent = CreateDefaultSubobject<USMainCameraComponent>(TEXT("Camera"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->FieldOfView = 120.f;
	CameraComponent->SpringArmComponent = SpringArmComponent;

	InitCameraPresets();
	CameraComponent->SetCameraPreset(0);

	GetCharacterMovement()->MaxAcceleration = 1400.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.4f;
}

ASBaseCharacter::~ASBaseCharacter()
{
	// if (GetWorld())
	// {
	// 	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	// }
}

void ASBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBaseCharacter::GiveAbilities()
{
	AquireAbility(ReloadAbility);
}

void ASBaseCharacter::ReduceStamina(float Value)
{
	AttributeSet->Stamina.SetCurrentValue(AttributeSet->Stamina.GetCurrentValue() - Value);
	if (AttributeSet->Stamina.GetCurrentValue() < 0.f)
		AttributeSet->Stamina.SetCurrentValue(0.f);
}

void ASBaseCharacter::AquireAbility(TSubclassOf<UGameplayAbility> Ability)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && Ability)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0));
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

bool ASBaseCharacter::IsDead() const
{
	return FMath::IsNearlyZero(AttributeSet->Health.GetCurrentValue());
}

void ASBaseCharacter::Jump()
{
	if (AttributeSet->Stamina.GetCurrentValue() < StaminaStats.Jump)
	{
		return;
	}
	ReduceStamina(StaminaStats.Jump);
	
	bIsWantToJump = true;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this]
	{
		bIsWantToJump = false;
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 0.1f, false);

	Super::Jump();
}

UAbilitySystemComponent* ASBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASBaseCharacter::SpawnWeapon()
{
	if (!GetWorld())
	{
		return;
	}

	WeaponBase = GetWorld()->SpawnActor<ASWeaponBase>(WeaponClass);
	if (WeaponBase)
	{
		WeaponBase->SetOwner(this);
		WeaponBase->AttachToComponent(GetMesh(), {EAttachmentRule::SnapToTarget, false}, "WeaponSocket");
	}
}

void ASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	ASBaseCharacter::SpawnWeapon();

	GiveAbilities();

	OnTakeAnyDamage.AddDynamic(this, &ASBaseCharacter::OnTakeAnyDamageHandler);

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(StaminaTimer, this, &ASBaseCharacter::RegenStamina, StaminaUpdateRate, true);
	}
}

void ASBaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ASBasePlayerController* CurrentController = Cast<ASBasePlayerController>(GetController());

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASBaseCharacter::Jump);
	PlayerInputComponent->BindAction("ChangeCameraView", IE_Pressed, CurrentController,
	                                 &ASBasePlayerController::ChangeCameraView);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, CurrentController,
	                                 &ASBasePlayerController::WantToStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, CurrentController,
	                                 &ASBasePlayerController::WantToStopSprint);
	PlayerInputComponent->BindAction("StopCameraControl", IE_Pressed, CurrentController,
	                                 &ASBasePlayerController::StartStopCameraControl);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, CurrentController,
	                                 &ASBasePlayerController::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, CurrentController,
	                                 &ASBasePlayerController::StopFire);
	PlayerInputComponent->BindAction("StopCameraControl", IE_Released, CurrentController,
	                                 &ASBasePlayerController::EndStopCameraControl);
	PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, CurrentController,
	                                 &ASBasePlayerController::ReloadWeapon);

	PlayerInputComponent->BindAxis("MoveForward", CurrentController, &ASBasePlayerController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", CurrentController, &ASBasePlayerController::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", CurrentController, &ASBasePlayerController::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &ASBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraDistance", CurrentController, &ASBasePlayerController::CameraDistance);
}

void ASBaseCharacter::RegenStamina()
{
	AttributeSet->Stamina.SetCurrentValue(AttributeSet->Stamina.GetCurrentValue() + StaminaRegenFactor);
	if (AttributeSet->Stamina.GetCurrentValue() > AttributeSet->Stamina.GetBaseValue())
		AttributeSet->Stamina.SetCurrentValue(AttributeSet->Stamina.GetBaseValue());
}

void ASBaseCharacter::InitCameraPresets()
{
	CameraComponent->CameraPresets.Add({
		"FPP",
		0.f,
		0.f,
		FVector(0.f, -1.f, -21.f),
		true,
		false
	});
	CameraComponent->CameraPresets.Add({
		"TPP",
		200.f,
		150.f,
		FVector(0.f, 0.f, 0.f),
		true,
		true,
		{
			true,
			true,
			15.f,
			15.f,
		},
		true
	});
}

void ASBaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}

	PlayAnimMontage(DeathAnimation);
	bIsDead = true;

	UnPossessed();
}

void ASBaseCharacter::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                             AController* InstigatedBy, AActor* DamageCauser)
{
	AttributeSet->Health.SetCurrentValue(AttributeSet->Health.GetCurrentValue() - Damage);
	const float Health = FMath::Clamp(AttributeSet->Health.GetCurrentValue(), 0.f, AttributeSet->Health.GetBaseValue());
	AttributeSet->Health.SetCurrentValue(Health);

	if (IsDead())
	{
		Dead();
	}
}

void ASBaseCharacter::Landed(const FHitResult& Hit)
{
	const UMainCharacterMovementComponent* MovementComponent = Cast<UMainCharacterMovementComponent>(
		GetCharacterMovement());

	if (!MovementComponent)
	{
		return;
	}

	const float Damage = MovementComponent->GetCountOfFallDamage();
	TakeDamage(Damage, FDamageEvent{}, Controller, this);

	if (Damage > 0.f)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), FallDamageSound);
	}

	Super::Landed(Hit);
}
