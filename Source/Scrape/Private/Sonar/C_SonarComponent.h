// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_SonarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_SonarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UC_SonarComponent();
    
    UFUNCTION(BlueprintCallable)
    bool FireSonar();
    
    UFUNCTION(BlueprintCallable)
    void FireFootstepSonar(float Speed);

    UPROPERTY(EditAnywhere, Category = "Sonar")
    TObjectPtr<UMaterialParameterCollection> MPC;

    // manual
    UPROPERTY(EditAnywhere, Category = "Sonar|Manual") float MaxRadius = 2000.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Manual") float Duration = 2.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Manual") float Cooldown = 4.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Manual") float NoiseCost = 0.05f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Manual") float SonarLoudness = 1.f;

    // foot step
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") bool  bAutoFootstep = true;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") bool  bSilentWhenCrouched = true;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float StrideLength = 180.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float MinSpeed = 50.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FullSpeed = 500.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FootstepMinRadius = 400.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FootstepMaxRadius = 900.f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FootstepDuration = 0.9f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FootstepStrength = 0.6f;
    UPROPERTY(EditAnywhere, Category = "Sonar|Footstep") float FootstepNoiseCost = 0.005f;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

private:
    struct FSlot
    {
        FVector Origin = FVector::ZeroVector;
        float Age = 0.f;
        float Duration = 1.f;
        float Radius = 0.f;
        float MaxR = 0.f;
        float Peak = 1.f;
        bool  bActive = false;
    };

    void StartSlot(int32 Index, const FVector& Origin, float InMaxR, float InDuration, float InPeak);
    void UpdateFootsteps(float DeltaTime);
    void WriteToMPC(const float Strength[4]);

    static const int32 ManualSlot = 0; //0 - manual 1-3 - footstep
    FSlot Slots[4];
    int32 NextFootSlot = 0;
    float CooldownLeft = 0.f;
    float StrideAccum = 0.f;
};
