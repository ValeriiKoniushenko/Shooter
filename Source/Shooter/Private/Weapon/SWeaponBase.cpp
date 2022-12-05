// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/SWeaponBase.h"
#include "Components/StaticMeshComponent.h"

ASWeaponBase::ASWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	check(StaticMeshComponent);
	StaticMeshComponent->SetupAttachment(GetRootComponent(), "WeaponSocket");
}

void ASWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
