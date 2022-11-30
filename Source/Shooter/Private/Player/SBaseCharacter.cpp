// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBaseCharacter.h"

#include "SBasePlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASBaseCharacter::ASBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->TargetArmLength = 500.f;
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
