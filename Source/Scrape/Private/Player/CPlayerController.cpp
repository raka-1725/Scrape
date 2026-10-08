// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerController.h"

#include "EnhancedInputSubsystems.h"

class UEnhancedInputLocalPlayerSubsystem;

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
