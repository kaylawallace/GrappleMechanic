// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrappleHookGameMode.h"
#include "GrappleHookCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrappleHookGameMode::AGrappleHookGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
