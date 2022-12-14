// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SReloadWeaponGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SBaseCharacter.h"

inline void USReloadWeaponGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                           const FGameplayAbilityActorInfo* ActorInfo,
                                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ActivateAbility!"));
	ASBaseCharacter* Character = Cast<ASBaseCharacter>(ActorInfo->OwnerActor);
	if (Character)
	{
		Character->WeaponBase->Reload();
	}
}
