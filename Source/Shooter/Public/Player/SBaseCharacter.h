// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CameraPreset.h"
#include "Weapon/SWeaponBase.h"
#include "SBaseCharacter.generated.h"

class SWeaponBase;
class UAbilitySystemComponent;
class USMainCharacterAttributeSet;
class USpringArmComponent;
class USMainCameraComponent;

UCLASS()
class SHOOTER_API ASBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASBaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FallDamage)
	float FallDamageAbsorption = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FallDamage)
	float FallDamageDevider = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	UAnimMontage* DeathAnimation;

	UPROPERTY(Category=Weapon, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASWeaponBase> WeaponClass;

	UPROPERTY()
	ASWeaponBase* WeaponBase = nullptr;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MinArmOffset = -200.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MaxArmOffset = 200.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MinCameraDistance = 100.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float MaxCameraDistance = 1500.f; // TODO: move to another class(new Camera class)

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	USMainCameraComponent* CameraComponent;

	UPROPERTY(Category=Camera, EditAnywhere)
	TArray<FCameraPreset> CameraPresets;

	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadWrite)
	bool bIsCanYaw = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRunForward = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsWantToJump = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Category=GAS, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> ReloadAbility;

	UPROPERTY(Category=GAS, EditAnywhere, BlueprintReadWrite)
	USMainCharacterAttributeSet* AttributeSet;

public:
	UFUNCTION(BlueprintCallable)
	void AquireAbility(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable)
	bool IsCanChangeSpringArmStats() const;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;

	void SwitchCameraMode();

	void GiveAbilities();

	FCameraPreset& GetCurrentCameraPreset();

	const FCameraPreset& GetCurrentCameraPreset() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta=(ClampMin=0, UIMin=0))
	int32 CurrentActiveCameraPreset = 0;

protected:
	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                            class AController* InstigatedBy, AActor* DamageCauser);

	virtual void Landed(const FHitResult& Hit) override;

	virtual void SpawnWeapon();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCameraPreset(uint32 Index);

private:
	void InitCameraPresets();

	void Dead();
	bool bIsDead = false;
};
