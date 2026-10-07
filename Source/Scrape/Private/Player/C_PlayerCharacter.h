// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_ScratchToolComponent.h"
#include "ScrapeCharacter.h"
#include "C_PlayerCharacter.generated.h"

class UC_SonarComponent;
/**
 * 
 */
UCLASS()
class AC_PlayerCharacter : public AScrapeCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Input") 
	TObjectPtr<UInputAction> IA_Scratch;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Sonar ;
	
	
	UPROPERTY(EditAnywhere, Category = "Components") 
	TObjectPtr<UC_ScratchToolComponent> ScratchTool;
		
	UPROPERTY(EditAnywhere, Category = "Components") 
	TObjectPtr<UC_SonarComponent> SonarComponent;
	
public:
	AC_PlayerCharacter();
protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void ScratchStart();
	void ScratchEnd();
};
