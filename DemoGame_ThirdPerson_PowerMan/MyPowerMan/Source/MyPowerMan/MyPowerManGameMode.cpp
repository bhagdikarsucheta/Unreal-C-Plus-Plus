// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPowerManGameMode.h"
#include "MyPowerManCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyPowerManGameMode::AMyPowerManGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
