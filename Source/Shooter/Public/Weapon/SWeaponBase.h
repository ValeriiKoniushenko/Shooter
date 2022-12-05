// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class SHOOTER_API ASWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ASWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
