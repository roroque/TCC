// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TccGameMode.h"
#include "TccPawn.h"

ATccGameMode::ATccGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATccPawn::StaticClass();
}

