// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MovementComponent/MainCharacterMovementComponent.h"
#include "Math/NumericLimits.h"

float UMainCharacterMovementComponent::GetCountOfFallDamage() const
{
	const float Gravity = FMath::Abs(GetGravityZ() / 100.f);
	const float Height = FMath::Abs((Velocity.Z / 100.f)); // 1m = 100cm
	const float Speed = FMath::Sqrt(Gravity * 2.f * Height);
	const float Momentum = Speed * Mass;
	float Damage = Momentum / FallDamageDevider - FallDamageAbsorption;
	return FMath::Clamp(Damage, 0.f, TNumericLimits<float>::Max());
}
