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

void ASWeaponBase::MakeShot()
{
	if (!GetWorld())
	{
		return;
	}

	ACharacter* CurrentPlayer = Cast<ACharacter>(GetOwner());
	
	if (!CurrentPlayer)
	{
		return;
	}

	AController* PlayerController = CurrentPlayer->GetController();
	if (!PlayerController)
	{
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);


	const FTransform SocketTransform = StaticMeshComponent->GetSocketTransform(BulletOutSocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector TraceEnd = TraceStart + ViewRotation.Vector() * FireDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.f, 3, 5.f);
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 3.f, 3, 5.f);
	}
}

void ASWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}
