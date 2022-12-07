// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTER_API ASWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ASWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=General)
	USkeletalMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=General)
	FName BulletOutSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting, meta=(UIMin=10, ClampMin=10))
	float FireDistance= 5000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting, meta=(UIMin=0, ClampMin=0))
	float RateOfFire = 0.07f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting)
	bool bIsAutomaticWeapon = true;

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();
	virtual void StopFire();

protected:
	virtual void Fire();
	virtual void BeginPlay() override;
	AController* GetController();
	FVector GetCameraHitLocation();

protected:
	FTimerHandle TimerHandler;
};
