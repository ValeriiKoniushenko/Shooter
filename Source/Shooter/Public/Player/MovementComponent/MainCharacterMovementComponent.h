// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UMainCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FallDamage)
	float FallDamageAbsorption = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FallDamage)
	float FallDamageDevider = 100.f;

	float GetCountOfFallDamage() const;
};
