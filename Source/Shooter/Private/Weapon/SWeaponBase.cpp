// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/SWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "SBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


ASWeaponBase::ASWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	check(StaticMeshComponent);
}

void ASWeaponBase::StartFire()
{
	GetWorldTimerManager().SetTimer(TimerHandler, this, &ASWeaponBase::Fire, RateOfFire, bIsAutomaticWeapon);
}

void ASWeaponBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandler);
}

void ASWeaponBase::Fire()
{
	const FTransform SocketTransform = StaticMeshComponent->GetSocketTransform(BulletOutSocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector TraceEnd = TraceStart + (GetCameraHitLocation() - TraceStart).GetSafeNormal() * FireDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10.f, 8, FColor::Red, false, 1.f, 3, 1.f);
	}
}

void ASWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

AController* ASWeaponBase::GetController()
{
	if (!GetWorld())
	{
		return nullptr;
	}

	ACharacter* CurrentPlayer = Cast<ACharacter>(GetOwner());

	if (!CurrentPlayer)
	{
		return nullptr;
	}

	return CurrentPlayer->GetController();
}

FVector ASWeaponBase::GetCameraHitLocation()
{
	AController* Controller = GetController();

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + ViewRotation.Vector() * 10000.f;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		return HitResult.Location;
	}
	return TraceEnd;
}
