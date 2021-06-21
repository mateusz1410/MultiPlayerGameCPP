// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

AShooterGameMode::AShooterGameMode()
{
	DefaultPawnClass = AShooterCharacter::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
}
