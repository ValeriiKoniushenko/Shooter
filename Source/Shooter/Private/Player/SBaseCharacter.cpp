// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBaseCharacter.h"

#include "SBasePlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASBaseCharacter::ASBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraPresets.Add({
		4.f,
		FVector(0.f, 0.f, 90.f),
		true
	});
	CameraPresets.Add({
		500.f,
		FVector(0.f, 0.f, 0.f),
		true
	});

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
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
}

void ASBaseCharacter::SetCameraPreset(uint32 Index)
{
	const FCameraPreset CameraPreset = CameraPresets[Index];

	SpringArmComponent->TargetArmLength = CameraPreset.TargetArmLength;
	SpringArmComponent->SetRelativeLocation(CameraPreset.RelativeLocation);
	SpringArmComponent->bUsePawnControlRotation = CameraPreset.bUsePawnControlRotation;
}
