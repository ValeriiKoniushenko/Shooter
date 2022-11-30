// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameModeBase.h"

#include "SBaseCharacter.h"
#include "SHUD.h"
#include "SPlayerState.h"

AShooterGameModeBase::AShooterGameModeBase()
{
	DefaultPawnClass = ASBaseCharacter::StaticClass();
	HUDClass = ASHUD::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
}
