// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MDPlayerController.generated.h"



class AMDCharacter;
class AMDTower;

/**
 * Controller for the player
 */
UCLASS()
class TCC_API AMDPlayerController : public APlayerController
{
	GENERATED_BODY()

	//method for setting up inputs
	virtual void SetupInputComponent() override;

public:

	//Reference to the player character
	AMDCharacter* PlayerCharacter;

	//Reference to the tower
	AMDTower* Tower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMDTower> TowerClass;

	bool bIsConstructing;

	float MovementForwardValue;
	float MovementRightValue;

	float ShootingForwardValue;
	float ShootingRightValue;

	//Axis functions:

	//movement joystick forward
	void LeftJoystickForward(float AxisValue);

	//movement joystick right
	void LeftJoystickRight(float AxisValue);

	//Shooting joystick forward
	void RightJoystickForward(float AxisValue);

	//shooting joystick right
	void RightJoystickRight(float AxisValue);
	
	UFUNCTION(BlueprintCallable)
	void StartConstructing();

	UFUNCTION(BlueprintCallable)
	bool Construct();
	
};
