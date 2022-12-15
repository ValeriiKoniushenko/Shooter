// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
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
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	USMainCameraComponent* CameraComponent;

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
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;

	void GiveAbilities();

protected:
	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                            class AController* InstigatedBy, AActor* DamageCauser);

	virtual void Landed(const FHitResult& Hit) override;

	virtual void SpawnWeapon();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitCameraPresets();

	void Dead();
	bool bIsDead = false;
};
