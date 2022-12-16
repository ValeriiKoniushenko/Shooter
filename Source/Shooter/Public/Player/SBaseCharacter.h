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
class USoundCue;

USTRUCT()
struct FStaminaCost
{
	GENERATED_BODY()
	
	float Run = .1f;
	float Sprint = .2f;
	float Jump = 10.f;
};

UCLASS()
class SHOOTER_API ASBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASBaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual ~ASBaseCharacter() override;
	
	UPROPERTY(Category=Stamina, EditAnywhere, BlueprintReadWrite)
	float StaminaRegenFactor = 5.f;
	
	UPROPERTY(Category=Stamina, EditAnywhere, BlueprintReadWrite)
	float StaminaUpdateRate = 1.f;
	
	UPROPERTY(Category=Animations, EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathAnimation;

	UPROPERTY(Category=Weapon, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASWeaponBase> WeaponClass;

	UPROPERTY(Category=Sounds, EditAnywhere, BlueprintReadWrite)
	USoundCue* FallDamageSound;

	UPROPERTY()
	ASWeaponBase* WeaponBase;

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

	FStaminaCost StaminaStats;
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

	void ReduceStamina(float Value);
protected:
	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                            class AController* InstigatedBy, AActor* DamageCauser);

	virtual void Landed(const FHitResult& Hit) override;

	virtual void SpawnWeapon();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RegenStamina();
	
private:
	void InitCameraPresets();

	void Dead();
	bool bIsDead = false;
	FTimerHandle StaminaTimer;
};
