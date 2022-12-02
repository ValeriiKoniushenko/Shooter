// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBaseCharacter.h"

#include "SBasePlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASBaseCharacter::ASBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh(), "HeadSocket");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	SetCameraPreset(0);

	GetCharacterMovement()->MaxAcceleration = 1000.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.4f;

	CameraComponent->FieldOfView = 120.f;
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

bool ASBaseCharacter::IsCanChangeSpringArmLength() const
{
	return GetCurrentCameraPreset().bIsCanChangeArmLength;
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

void ASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASBaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ASBasePlayerController* CurrentController = Cast<ASBasePlayerController>(GetController());

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASBaseCharacter::Jump);
	PlayerInputComponent->BindAction("ChangeCameraView", IE_Pressed, CurrentController, &ASBasePlayerController::ChangeCameraView);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, CurrentController, &ASBasePlayerController::WantToStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, CurrentController, &ASBasePlayerController::WantToStopSprint);
	PlayerInputComponent->BindAxis("MoveForward", CurrentController, &ASBasePlayerController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", CurrentController, &ASBasePlayerController::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &ASBaseCharacter::AddControllerYawInput);
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
