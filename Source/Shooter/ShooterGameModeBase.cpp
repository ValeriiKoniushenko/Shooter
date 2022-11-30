// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameModeBase.h"

#include "SBaseCharacter.h"
#include "SBasePlayerController.h"
#include "SHUD.h"
#include "SPlayerState.h"

AShooterGameModeBase::AShooterGameModeBase()
{
	DefaultPawnClass = ASBaseCharacter::StaticClass();
	HUDClass = ASHUD::StaticClass();
	PlayerControllerClass = ASBasePlayerController::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
}
