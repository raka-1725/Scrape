// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACPlayerController();
	
private:
	UPROPERTY(EditAnywhere, Category="Input | IMC")
	class UInputMappingContext* DefaultMappingContext;
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
