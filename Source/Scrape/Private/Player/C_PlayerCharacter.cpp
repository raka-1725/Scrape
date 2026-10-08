// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Sonar/C_SonarComponent.h"

AC_PlayerCharacter::AC_PlayerCharacter()
{
	ScratchTool = CreateDefaultSubobject<UC_ScratchToolComponent>(TEXT("ScratchTool"));
	SonarComponent = CreateDefaultSubobject<UC_SonarComponent>(TEXT("SonarComponent"));
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
	UE_LOG(LogTemp, Warning, TEXT("Scratch Start"));
}
void AC_PlayerCharacter::ScratchEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Scratch End"));
	ScratchTool->SetScratching(false);
}