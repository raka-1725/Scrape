// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/NoiseSystem.h"

#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"


bool UNoiseSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void UNoiseSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	MPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/_MyFiles/Shaders/MPC_Game.MPC_Game"));
	if (!MPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("MPC not found"));
	}
}


void UNoiseSystem::AddNoise(float Amount)
{
	if (Amount <= 0.0f) { return; }
	NoiseLevel = FMath::Clamp(NoiseLevel + Amount, 0.0f, 1.0f);
	TimeSinceAdd = 0.0f; 
}

float UNoiseSystem::GetGrowMultiplier() const
{
	if (NoiseLevel > 0.85f) { return 0.f;  } 
	if (NoiseLevel > 0.6f)  { return 1.3f; } 
	return 1.f;
}

float UNoiseSystem::GetSonarRadiusScale() const
{
	return 1.0f - NoiseLevel * 0.5f;
}


void UNoiseSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeSinceAdd += DeltaTime;
	if (TimeSinceAdd > RecoveryDelay)
	{
		NoiseLevel = FMath::Max(0.0f, NoiseLevel - RecoveryPerSecond * DeltaTime);
	}
	
	if (NoiseLevel >= 1.f && !bMax)
	{
		bMax = true;
		OnNoiseMaxed.Broadcast();
	}
	else if (NoiseLevel < 0.8f)
	{
		bMax = false;
	}

	DisplayNoise = FMath::FInterpTo(DisplayNoise, NoiseLevel, DeltaTime, VisualInterpSpeed);

	PushToMPC();
	OnNoiseChanged.Broadcast(NoiseLevel);
}

void UNoiseSystem::PushToMPC()
{
	if (!MPC || !GetWorld()) { return; }
	if (UMaterialParameterCollectionInstance* Inst = GetWorld()->GetParameterCollectionInstance(MPC))
	{
		Inst->SetScalarParameterValue(FName(TEXT("NoiseLevel")), DisplayNoise);
	}
}
