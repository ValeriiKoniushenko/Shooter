// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/SWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "SBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimMontage.h"

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

void ASWeaponBase::Reload()
{
	if (TotalCountOfBullets <= 0 || BulletsPerMagazine == BulletsPerMagazineNow)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
	const int32 GotFromMagazine = FMath::Clamp(TotalCountOfBullets, 0, BulletsPerMagazine - BulletsPerMagazineNow);
	BulletsPerMagazineNow += GotFromMagazine;
	TotalCountOfBullets -= GotFromMagazine;
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(ReloadAnimation);

	UE_LOG(LogTemp, Warning, TEXT("%d %d"), BulletsPerMagazineNow, TotalCountOfBullets);
}

void ASWeaponBase::Fire()
{
	if (BulletsPerMagazineNow <= 0)
	{
		return;
	}

	const AController* Controller = GetController();

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector Direction = ViewRotation.Vector();
	Direction = FMath::VRandCone(Direction, FMath::DegreesToRadians(Spread), FMath::DegreesToRadians(Spread));

	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + Direction * FireDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);

	if (HitResult.bBlockingHit && !HitResult.Actor.Get())
	{
		UGameplayStatics::SpawnDecalAttached(BulletHoleDecal, DecalSize, HitResult.GetComponent(),
		                                     NAME_None,
		                                     HitResult.Location, HitResult.Normal.Rotation(),
		                                     EAttachLocation::KeepRelativeOffset,
		                                     DecalLifeSpan);
	}

	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	--BulletsPerMagazineNow;
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
