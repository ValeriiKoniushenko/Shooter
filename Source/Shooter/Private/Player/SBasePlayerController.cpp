// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBasePlayerController.h"
#include "SBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASBasePlayerController::MoveForward(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Value, true);
	CurrentPlayer->bIsRunForward = Value > 0.f;
}

void ASBasePlayerController::MoveRight(float Value)
{
	ACharacter* CurrentPlayer = GetCharacter();
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorRightVector(), Value, true);
}

void ASBasePlayerController::StartStopCameraControl()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->bIsCanYaw = false;
}

void ASBasePlayerController::EndStopCameraControl()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->bIsCanYaw = true;
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

	if (!FMath::IsNearlyZero(Value) && IsInputKeyDown(EKeys::LeftAlt) && CurrentPlayer->IsCanChangeSpringArmStats())
	{
		float& ArmLength = CurrentPlayer->SpringArmComponent->TargetArmLength;

		CurrentPlayer->GetCurrentCameraPreset().TargetArmLength += Value;
		ArmLength += Value;
		ArmLength = FMath::Clamp(ArmLength, CurrentPlayer->MinCameraDistance, CurrentPlayer->MaxCameraDistance);
	}
}

void ASBasePlayerController::MouseX(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());

	if (CurrentPlayer->bIsCanYaw)
	{
		CurrentPlayer->AddControllerYawInput(Value);
	}

	if (!FMath::IsNearlyZero(Value) && IsInputKeyDown(EKeys::LeftAlt) && CurrentPlayer->IsCanChangeSpringArmStats())
	{
		FVector& SocketOffset = CurrentPlayer->SpringArmComponent->SocketOffset;

		CurrentPlayer->GetCurrentCameraPreset().ArmOffset += Value;
		SocketOffset.Y += Value;
		SocketOffset.Y = FMath::Clamp(SocketOffset.Y, CurrentPlayer->MinArmOffset, CurrentPlayer->MaxArmOffset);
	}
}
