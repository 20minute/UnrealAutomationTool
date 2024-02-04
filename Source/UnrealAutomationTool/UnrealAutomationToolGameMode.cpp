// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealAutomationToolGameMode.h"
#include "UnrealAutomationToolCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealAutomationToolGameMode::AUnrealAutomationToolGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
