// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimuWare_ue4GameMode.h"
#include "SimuWare_ue4HUD.h"
#include "SimuWare_ue4Character.h"
#include "UObject/ConstructorHelpers.h"

ASimuWare_ue4GameMode::ASimuWare_ue4GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASimuWare_ue4HUD::StaticClass();
}
