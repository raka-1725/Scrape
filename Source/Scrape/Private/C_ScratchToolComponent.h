// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_ScratchToolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_ScratchToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UC_ScratchToolComponent();

	UFUNCTION(BlueprintCallable) void SetScratching(bool bNew) { bScratching = bNew; }
	UFUNCTION(BlueprintPure) bool IsScratching() const { return bScratching; }

	UPROPERTY(EditAnywhere) float Reach = 250.f;
	UPROPERTY(EditAnywhere) float NoisePerSecond = 0.08f;
	UPROPERTY(EditAnywhere) float ScratchMoveSpeedScale = 0.4f;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bScratching = false;
	bool bWasScratching = false;
	float CachedWalkSpeed = 0.f;	
};
