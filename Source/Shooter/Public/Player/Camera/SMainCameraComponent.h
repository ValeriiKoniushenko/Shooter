// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Player/CameraPreset.h"
#include "SMainCameraComponent.generated.h"

class USpringArmComponent;

UCLASS()
class SHOOTER_API USMainCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MinArmOffset = -200.f;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MaxArmOffset = 200.f;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MinCameraDistance = 100.f;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MaxCameraDistance = 1500.f;

	UPROPERTY(Category=Camera, EditAnywhere)
	TArray<FCameraPreset> CameraPresets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

public:
	UFUNCTION(BlueprintCallable)
	bool IsCanChangeSpringArmStats() const;

	void SwitchCameraMode();

	FCameraPreset& GetCurrentCameraPreset();

	const FCameraPreset& GetCurrentCameraPreset() const;

	void SetCameraPreset(uint32 Index);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta=(ClampMin=0, UIMin=0))
	int32 CurrentActiveCameraPreset = 0;
};
