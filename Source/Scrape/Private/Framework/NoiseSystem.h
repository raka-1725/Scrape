// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NoiseSystem.generated.h"

class UMaterialParameterCollection;
/**
 * 
 */
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoiseChanged, float, NewNoise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoiseMaxed);

UCLASS()
class UNoiseSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const final override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) final override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UNoiseSystem, STATGROUP_Tickables);
	}

	UFUNCTION(BlueprintCallable) void AddNoise(float Amount);
	UFUNCTION(BlueprintCallable) void SetNoise(float NewValue);
	UFUNCTION(BlueprintPure) float GetNoise() const { return NoiseLevel; }
	
	UFUNCTION(BlueprintPure) float GetGrowMultiplier() const;
	UFUNCTION(BlueprintPure) float GetSonarRadiusScale() const;
	
	UPROPERTY(BlueprintAssignable) FOnNoiseChanged OnNoiseChanged;
	UPROPERTY(BlueprintAssignable) FOnNoiseMaxed OnNoiseMaxed;

	UPROPERTY(EditAnywhere, Category = "Noise") float RecoveryPerSecond = 0.03f;
	UPROPERTY(EditAnywhere, Category = "Noise") float RecoveryDelay = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Noise") float VisualInterpSpeed = 6.f;
	
	
private:
	void PushToMPC();
	
	UPROPERTY() TObjectPtr<UMaterialParameterCollection> MPC;
	
	float NoiseLevel = 0.0f;
	float DisplayNoise = 0.0f;
	float TimeSinceAdd = 0.0f;
	bool bMax = false;
};

