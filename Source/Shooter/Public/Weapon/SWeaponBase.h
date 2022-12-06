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
	float FireDistance;

	UFUNCTION(BlueprintCallable)
	virtual void MakeShot();
protected:
	virtual void BeginPlay() override;
	AController* GetController();
	FVector GetCameraHitLocation();
};
