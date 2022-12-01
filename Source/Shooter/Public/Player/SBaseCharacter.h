// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CameraPreset.h"
#include "SBaseCharacter.generated.h"

class USpringArmComponent;

UCLASS()
class SHOOTER_API ASBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASBaseCharacter();

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MinCameraDistance = 100.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MaxCameraDistance = 1500.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(Category=Camera, EditAnywhere)
	TArray<FCameraPreset> CameraPresets;

	virtual void Tick(float DeltaTime) override;

	void SwitchCameraMode();

	UFUNCTION(BlueprintCallable)
	bool IsCanChangeSpringArmLength() const;

	FCameraPreset& GetCurrentCameraPreset();
	const FCameraPreset& GetCurrentCameraPreset() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRunForward = false;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCameraPreset(uint32 Index);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta=(ClampMin=0, UIMin=0))
	int32 CurrentActiveCameraPreset = 0;

};
