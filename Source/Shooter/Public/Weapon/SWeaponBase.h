// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponBase.generated.h"

class USkeletalMeshComponent;
class UMaterialInterface;
class USoundCue;

UCLASS()
class SHOOTER_API ASWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ASWeaponBase();

	UFUNCTION(BlueprintCallable)
	float GetBulletSpread();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=General)
	USkeletalMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=General)
	FName BulletOutSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting, meta=(UIMin=10, ClampMin=10))
	float FireDistance = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting, meta=(UIMin=0, ClampMin=0))
	float RateOfFire = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting, meta=(UIMin=0, ClampMin=0))
	float Spread = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting)
	bool bIsAutomaticWeapon = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting)
	int32 BulletsPerMagazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting)
	int32 BulletsPerMagazineNow = BulletsPerMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Shooting)
	int32 TotalCountOfBullets = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Decals)
	UMaterialInterface* BulletHoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Decals)
	float DecalLifeSpan = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Decals)
	FVector DecalSize = FVector(8.0f, 8.0f, 8.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sounds)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sounds)
	USoundCue* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	UAnimMontage* ReloadAnimation;

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();
	virtual void StopFire();
	virtual bool Reload();

	UPROPERTY()
	bool bIsCanShoot = true;

protected:
	virtual void Fire();
	virtual void BeginPlay() override;
	AController* GetController();

protected:
	FTimerHandle TimerHandler;

private:
	bool bIsWantToFire = true;
};
