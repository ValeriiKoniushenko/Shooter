// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SBasePlayerController.h"
#include "GAS/SMainCharacterAttributeSet.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/SMainCameraComponent.h"

ASBaseCharacter::ASBaseCharacter()
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

	InitCameraPresets();
	SetCameraPreset(0);

	GetCharacterMovement()->MaxAcceleration = 1400.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.4f;
}

void ASBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBaseCharacter::SwitchCameraMode()
{
	CurrentActiveCameraPreset++;
	if (CurrentActiveCameraPreset >= CameraPresets.Num())
	{
		CurrentActiveCameraPreset = 0;
	}

	SetCameraPreset(CurrentActiveCameraPreset);
}

void ASBaseCharacter::GiveAbilities()
{
	AquireAbility(ReloadAbility);
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

bool ASBaseCharacter::IsCanChangeSpringArmStats() const
{
	return GetCurrentCameraPreset().bIsCanChangeArmLength;
}

bool ASBaseCharacter::IsDead() const
{
	return FMath::IsNearlyZero(AttributeSet->Health.GetCurrentValue());
}

FCameraPreset& ASBaseCharacter::GetCurrentCameraPreset()
{
	return CameraPresets[CurrentActiveCameraPreset];
}

const FCameraPreset& ASBaseCharacter::GetCurrentCameraPreset() const
{
	return CameraPresets[CurrentActiveCameraPreset];
}

void ASBaseCharacter::Jump()
{
	Super::Jump();

	bIsWantToJump = true;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this]
	{
		bIsWantToJump = false;
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 0.1f, false);
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

void ASBaseCharacter::SetCameraPreset(uint32 Index)
{
	const FCameraPreset CameraPreset = CameraPresets[Index];

	SpringArmComponent->TargetArmLength = CameraPreset.TargetArmLength;
	SpringArmComponent->SocketOffset = FVector(0.f, CameraPreset.ArmOffset, 0.f);
	SpringArmComponent->SetRelativeLocation(CameraPreset.RelativeLocation);
	SpringArmComponent->bUsePawnControlRotation = CameraPreset.bUsePawnControlRotation;
	GetCharacterMovement()->bUseControllerDesiredRotation = CameraPreset.bUseControllerDesiredRotation;
	if (CameraPreset.Lag.bIsEnabled)
	{
		SpringArmComponent->bEnableCameraLag = true;
		SpringArmComponent->CameraLagSpeed = CameraPreset.Lag.Speed;
		SpringArmComponent->bEnableCameraRotationLag = CameraPreset.Lag.bIsRotation;
		SpringArmComponent->CameraRotationLagSpeed = CameraPreset.Lag.RotationSpeed;
	}
	else
	{
		SpringArmComponent->bEnableCameraLag = false;
	}
}

void ASBaseCharacter::InitCameraPresets()
{
	CameraPresets.Add({
		"FPP",
		0.f,
		0.f,
		FVector(0.f, -1.f, -21.f),
		true,
		false
	});
	CameraPresets.Add({
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
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	const float Gravity = FMath::Abs(MovementComponent->GetGravityZ() / 100.f);
	const float Height = FMath::Abs((MovementComponent->Velocity.Z / 100.f)); // 1m = 100cm
	const float Speed = FMath::Sqrt(Gravity * 2.f * Height);
	const float Momentum = Speed * MovementComponent->Mass;
	float Damage = Momentum / FallDamageDevider - FallDamageAbsorption;
	Damage = FMath::Clamp(Damage, 0.f, 10000000.f);
	TakeDamage(Damage, FDamageEvent{}, Controller, this);
	UE_LOG(LogTemp, Warning, TEXT("Fallen from: %f ; Damage: %f"), MovementComponent->Velocity.Z, Damage);

	Super::Landed(Hit);
}
