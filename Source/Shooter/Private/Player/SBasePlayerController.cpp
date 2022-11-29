// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SBasePlayerController.h"
#include "SBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASBasePlayerController::MoveForward(float Value)
{
	ACharacter* CurrentPlayer = GetCharacter();
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Value);
}

void ASBasePlayerController::MoveRight(float Value)
{
	ACharacter* CurrentPlayer = GetCharacter();
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorRightVector(), Value);
}

void ASBasePlayerController::ChangeCameraView()
{
	UE_LOG(LogTemp, Error, TEXT("Try to change camera view"));
}

void ASBasePlayerController::WantToStartSprint()
{
	LastMaxWalkSpeed = GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	bIsSprinting = true;
}

void ASBasePlayerController::WantToStopSprint()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = LastMaxWalkSpeed;
	bIsSprinting = false;
}
