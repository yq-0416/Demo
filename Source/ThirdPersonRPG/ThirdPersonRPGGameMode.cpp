// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonRPGGameMode.h"
#include "ThirdPersonRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonRPGGameMode::AThirdPersonRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
