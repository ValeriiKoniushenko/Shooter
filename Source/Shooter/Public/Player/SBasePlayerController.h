// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ASBasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MaxSprintSpeed = 1200.f;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void StartStopCameraControl();
	virtual void EndStopCameraControl();
	virtual void ChangeCameraView();
	virtual void WantToStartSprint();
	virtual void WantToStopSprint();
	virtual void CameraDistance(float Value);
	virtual void MouseX(float Value);
private:
	float LastMaxWalkSpeed = 0;
	bool bIsSprinting = false;
};
