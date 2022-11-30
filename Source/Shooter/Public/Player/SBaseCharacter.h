// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SBaseCharacter.generated.h"

class USpringArmComponent;

UCLASS()
class SHOOTER_API ASBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASBaseCharacter();

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
