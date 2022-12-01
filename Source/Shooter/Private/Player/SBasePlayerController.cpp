// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBasePlayerController.h"
#include "SBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASBasePlayerController::MoveForward(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Value);
	CurrentPlayer->bIsRunForward = !FMath::IsNearlyZero(Value);
}

void ASBasePlayerController::MoveRight(float Value)
{
	ACharacter* CurrentPlayer = GetCharacter();
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorRightVector(), Value);
}

void ASBasePlayerController::ChangeCameraView()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->SwitchCameraMode();
}

void ASBasePlayerController::WantToStartSprint()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());

	if (CurrentPlayer->bIsRunForward)
	{
		LastMaxWalkSpeed = CurrentPlayer->GetCharacterMovement()->MaxWalkSpeed;
		CurrentPlayer->GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
		bIsSprinting = true;
	}
}

void ASBasePlayerController::WantToStopSprint()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = LastMaxWalkSpeed;
	bIsSprinting = false;
}

void ASBasePlayerController::CameraDistance(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());

	if (!FMath::IsNearlyZero(Value) && IsInputKeyDown(EKeys::LeftAlt) && CurrentPlayer->IsCanChangeSpringArmLength())
	{
		float& ArmLength = CurrentPlayer->SpringArmComponent->TargetArmLength;

		CurrentPlayer->GetCurrentCameraPreset().TargetArmLength += Value;
		ArmLength += Value;
		ArmLength = FMath::Clamp(ArmLength, CurrentPlayer->MinCameraDistance, CurrentPlayer->MaxCameraDistance);
	}
}
