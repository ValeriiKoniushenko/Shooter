// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBasePlayerController.h"
#include "SBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/SMainCameraComponent.h"
#include "GAS/SMainCharacterAttributeSet.h"

void ASBasePlayerController::MoveForward(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Value, true);
	CurrentPlayer->bIsRunForward = Value > 0.f;

	if (!FMath::IsNearlyZero(Value))
	{
		if (bIsSprinting)
		{
			CurrentPlayer->ReduceStamina(CurrentPlayer->StaminaStats.Sprint);
		}
		else
		{
			CurrentPlayer->ReduceStamina(CurrentPlayer->StaminaStats.Run);
		}
	}

	if (CurrentPlayer->AttributeSet->Stamina.GetCurrentValue() < 5)
	{
		WantToStopSprint();
	}
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
	CurrentPlayer->CameraComponent->SwitchCameraMode();
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

	if (!FMath::IsNearlyZero(Value) && IsInputKeyDown(EKeys::LeftAlt) && CurrentPlayer->CameraComponent->
		IsCanChangeSpringArmStats())
	{
		float& ArmLength = CurrentPlayer->SpringArmComponent->TargetArmLength;

		CurrentPlayer->CameraComponent->GetCurrentCameraPreset().TargetArmLength += Value;
		ArmLength += Value;
		ArmLength = FMath::Clamp(ArmLength, CurrentPlayer->CameraComponent->MinCameraDistance,
		                         CurrentPlayer->CameraComponent->MaxCameraDistance);
	}
}

void ASBasePlayerController::MouseX(float Value)
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());

	if (CurrentPlayer->bIsCanYaw)
	{
		CurrentPlayer->AddControllerYawInput(Value);
	}

	if (!FMath::IsNearlyZero(Value) && IsInputKeyDown(EKeys::LeftAlt) && CurrentPlayer->CameraComponent->
		IsCanChangeSpringArmStats())
	{
		FVector& SocketOffset = CurrentPlayer->SpringArmComponent->SocketOffset;

		CurrentPlayer->CameraComponent->GetCurrentCameraPreset().ArmOffset += Value;
		SocketOffset.Y += Value * 2.f;
		SocketOffset.Y = FMath::Clamp(SocketOffset.Y, CurrentPlayer->CameraComponent->MinArmOffset,
		                              CurrentPlayer->CameraComponent->MaxArmOffset);
	}
}

void ASBasePlayerController::StartFire()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	if (CurrentPlayer->WeaponBase)
	{
		CurrentPlayer->WeaponBase->StartFire();
	}
}

void ASBasePlayerController::StopFire()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	if (CurrentPlayer->WeaponBase)
	{
		CurrentPlayer->WeaponBase->StopFire();
	}
}

void ASBasePlayerController::ReloadWeapon()
{
	ASBaseCharacter* CurrentPlayer = Cast<ASBaseCharacter>(GetCharacter());
	if (CurrentPlayer->WeaponBase)
	{
		CurrentPlayer->WeaponBase->Reload();
	}
}
