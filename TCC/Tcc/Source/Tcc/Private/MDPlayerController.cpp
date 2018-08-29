// Fill out your copyright notice in the Description page of Project Settings.

#include "MDPlayerController.h"
#include "MDCharacter.h"
#include "MDTower.h"

void AMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//bind axis events
	InputComponent->BindAxis("MoveForward", this, &AMDPlayerController::LeftJoystickForward);
	InputComponent->BindAxis("MoveRight", this, &AMDPlayerController::LeftJoystickRight);
	InputComponent->BindAxis("FireForward", this, &AMDPlayerController::RightJoystickForward);
	InputComponent->BindAxis("FireRight", this, &AMDPlayerController::RightJoystickRight);

}

void AMDPlayerController::LeftJoystickForward(float AxisValue)
{
	if (bIsConstructing)
	{
		
	}
	MovementForwardValue = AxisValue;

}

void AMDPlayerController::LeftJoystickRight(float AxisValue)
{
	if (bIsConstructing)
	{
		
	}
	MovementRightValue = AxisValue;

}

void AMDPlayerController::RightJoystickForward(float AxisValue)
{
	if (bIsConstructing)
	{
		if (Tower)
		{
			Tower->MoveHorizontal(AxisValue);
		}
		ShootingForwardValue = 0;
		return;
	}
	ShootingForwardValue = AxisValue;

}

void AMDPlayerController::RightJoystickRight(float AxisValue)
{
	if (bIsConstructing)
	{
		if (Tower)
		{
			Tower->MoveVertical(AxisValue);
		}
		ShootingRightValue = 0;
		return;
	}
	ShootingRightValue = AxisValue;

}

void AMDPlayerController::StartConstructing()
{
	bIsConstructing = true;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = NULL;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (!PlayerCharacter)
		{
			return;
		}

		FTransform SpawnTowerTransform = PlayerCharacter->GetActorTransform();

		FVector NewLocation = SpawnTowerTransform.GetLocation();
		NewLocation.X += 100;
		NewLocation.Z += 30;


		SpawnTowerTransform.SetLocation(NewLocation);

		Tower = World->SpawnActor<AMDTower>(TowerClass,SpawnTowerTransform, SpawnParams);
		
	}


}

bool AMDPlayerController::Construct()
{
	if (Tower && Tower->IsAbleToConstruct())
	{
		Tower->TowerOwner = PlayerCharacter;
		Tower->Construct();
		Tower = nullptr;
		bIsConstructing = false;

		return true;
	}
	return false;
}
