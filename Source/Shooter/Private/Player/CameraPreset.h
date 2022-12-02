#pragma once

#include "Misc/Optional.h"
#include "CameraPreset.generated.h"

USTRUCT()
struct FCameraLag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsEnabled = false;

	UPROPERTY(EditAnywhere)
	bool bIsRotation = false;

	UPROPERTY(EditAnywhere)
	float Speed = 10.f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 10.f;
};

USTRUCT()
struct FCameraPreset
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	float TargetArmLength = 0;

	UPROPERTY(EditAnywhere)
	float ArmOffset = 0;

	UPROPERTY(EditAnywhere)
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere)
	bool bUsePawnControlRotation = true;

	UPROPERTY(EditAnywhere)
	bool bUseControllerDesiredRotation = true;

	UPROPERTY(EditAnywhere)
	FCameraLag Lag = {};

	UPROPERTY(EditAnywhere)
	bool bIsCanChangeArmLength = false;
};
