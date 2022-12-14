// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "SMainCharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API USMainCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Health = 100.f;
};
