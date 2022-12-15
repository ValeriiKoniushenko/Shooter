// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Camera/SMainCameraComponent.h"

#include "SBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "SBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

FCameraPreset& USMainCameraComponent::GetCurrentCameraPreset()
{
	return CameraPresets[CurrentActiveCameraPreset];
}

const FCameraPreset& USMainCameraComponent::GetCurrentCameraPreset() const
{
	return CameraPresets[CurrentActiveCameraPreset];
}

void USMainCameraComponent::SetCameraPreset(uint32 Index)
{
	const FCameraPreset CameraPreset = CameraPresets[Index];

	SpringArmComponent->TargetArmLength = CameraPreset.TargetArmLength;
	SpringArmComponent->SocketOffset = FVector(0.f, CameraPreset.ArmOffset, 0.f);
	SpringArmComponent->SetRelativeLocation(CameraPreset.RelativeLocation);
	SpringArmComponent->bUsePawnControlRotation = CameraPreset.bUsePawnControlRotation;
	Cast<ASBaseCharacter>(GetOwner())->GetCharacterMovement()->bUseControllerDesiredRotation = CameraPreset.bUseControllerDesiredRotation;
	
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

void USMainCameraComponent::SwitchCameraMode()
{
	CurrentActiveCameraPreset++;
	if (CurrentActiveCameraPreset >= CameraPresets.Num())
	{
		CurrentActiveCameraPreset = 0;
	}

	SetCameraPreset(CurrentActiveCameraPreset);
}

bool USMainCameraComponent::IsCanChangeSpringArmStats() const
{
	return GetCurrentCameraPreset().bIsCanChangeArmLength;
}