// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerCharacter.h"

#include "EnhancedInputComponent.h"

AC_PlayerCharacter::AC_PlayerCharacter()
{
	ScratchTool = CreateDefaultSubobject<UC_ScratchToolComponent>(TEXT("ScratchTool"));
	
}

void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(IA_Scratch, ETriggerEvent::Started,   this, &AC_PlayerCharacter::ScratchStart);
		EIC->BindAction(IA_Scratch, ETriggerEvent::Completed, this, &AC_PlayerCharacter::ScratchEnd);
		EIC->BindAction(IA_Scratch, ETriggerEvent::Canceled,  this, &AC_PlayerCharacter::ScratchEnd);
	}
}

void AC_PlayerCharacter::ScratchStart()
{
	ScratchTool->SetScratching(true);
}
void AC_PlayerCharacter::ScratchEnd()
{
	ScratchTool->SetScratching(false);
}