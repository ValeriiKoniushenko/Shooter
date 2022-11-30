﻿#pragma once

#include "CameraPreset.generated.h"

USTRUCT()
struct FCameraPreset
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	float TargetArmLength = 0;

	UPROPERTY(EditAnywhere)
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere)
	bool bUsePawnControlRotation = true;

	UPROPERTY(EditAnywhere)
	bool bUseControllerDesiredRotation = true;
};