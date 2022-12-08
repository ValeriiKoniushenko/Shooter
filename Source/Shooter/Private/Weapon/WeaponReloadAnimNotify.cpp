// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponReloadAnimNotify.h"
#include "SBaseCharacter.h"

void UWeaponReloadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAnimNotify::Notify(MeshComp, Animation);

	ASBaseCharacter* Player = Cast<ASBaseCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->WeaponBase->bIsCanShoot = true;
}
