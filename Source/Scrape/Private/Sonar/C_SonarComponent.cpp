// Fill out your copyright notice in the Description page of Project Settings.


#include "Sonar/C_SonarComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
UC_SonarComponent::UC_SonarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_SonarComponent::BeginPlay()
{
	Super::BeginPlay();
	MPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/_MyFiles/Shaders/MPC_Game.MPC_Game"));
}
void UC_SonarComponent::StartSlot(int32 Index, const FVector& Origin, float InMaxR, float InDuration, float InPeak)
{
	FSlot& S = Slots[Index];
	S.Origin = Origin;
	S.Age = 0.f;
	S.Radius = 0.f;
	S.MaxR = InMaxR;
	S.Duration = FMath::Max(InDuration, 0.01f);
	S.Peak = InPeak;
	S.bActive = true;
}
bool UC_SonarComponent::FireSonar()
{
	if (CooldownLeft > 0.f) return false;
	
	//UNoiseSubsystem* Noise = GetWorld()->GetSubsystem<UMNNoiseSubsystem>();
	//const float Scale = Noise ? Noise->GetSonarRadiusScale() : 1.f;
	const float Scale = 1.0f;
	
	StartSlot(ManualSlot, GetOwner()->GetActorLocation(), MaxRadius * Scale, Duration, 1.f);
	CooldownLeft = Cooldown;

	//if (Noise) Noise->AddNoise(NoiseCost);

	//UAISense_Hearing::ReportNoiseEvent(GetWorld(), S.Origin, SonarLoudness,GetOwner(), 0.f, TEXT("Sonar"));
	return true;
}

void UC_SonarComponent::FireFootstepSonar(float Speed)
{
	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char) { return; }

	const float Alpha = FMath::Clamp(Speed / FullSpeed, 0.f, 1.f);
	const float R = FMath::Lerp(FootstepMinRadius, FootstepMaxRadius, Alpha);

	
	const FVector Feet = Char->GetActorLocation()
					   - FVector(0.f, 0.f, Char->GetSimpleCollisionHalfHeight());

	const int32 Index = 1 + NextFootSlot;
	NextFootSlot = (NextFootSlot + 1) % 3;

	StartSlot(Index, Feet, R, FootstepDuration, FootstepStrength * FMath::Lerp(0.6f, 1.f, Alpha));

	// if (UNoiseSubsystem* Noise = GetWorld()->GetSubsystem<UNoiseSubsystem>())
	//     Noise->AddNoise(FootstepNoiseCost * Alpha);
	// UAISense_Hearing::ReportNoiseEvent(GetWorld(), Feet, 0.3f * Alpha, GetOwner(), 0.f, TEXT("Footstep"));
}

void UC_SonarComponent::UpdateFootsteps(float DeltaTime)
{
	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	const UCharacterMovementComponent* Move = Char ? Char->GetCharacterMovement() : nullptr;
	if (!Move) { return; }

	if (!Move->IsMovingOnGround() || (bSilentWhenCrouched && Move->IsCrouching()))
	{
		return;
	}

	const float Speed = Char->GetVelocity().Size2D();
	if (Speed < MinSpeed)
	{
		return;
	}

	StrideAccum += Speed * DeltaTime;
	if (StrideAccum >= StrideLength)
	{
		StrideAccum -= StrideLength;
		FireFootstepSonar(Speed);
	}
}

void UC_SonarComponent::WriteToMPC(const float Strength[4])
{
	UMaterialParameterCollectionInstance* Inst = GetWorld()->GetParameterCollectionInstance(MPC);
	if (!Inst) { return; }

	static const FName SonarNames[4] = {
		FName(TEXT("Sonar0")), FName(TEXT("Sonar1")),
		FName(TEXT("Sonar2")), FName(TEXT("Sonar3")) };

	for (int32 i = 0; i < 4; ++i)
	{
		const FSlot& S = Slots[i];
		Inst->SetVectorParameterValue(SonarNames[i],
			FLinearColor(S.Origin.X, S.Origin.Y, S.Origin.Z, S.Radius));
	}
	
	Inst->SetVectorParameterValue(FName(TEXT("SonarStrength")),
		FLinearColor(Strength[0], Strength[1], Strength[2], Strength[3]));
}

void UC_SonarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	CooldownLeft = FMath::Max(0.f, CooldownLeft - DeltaTime);

	if (bAutoFootstep)
	{
		UpdateFootsteps(DeltaTime);
	}

	float Strength[4] = { 0.f, 0.f, 0.f, 0.f };

	for (int32 i = 0; i < 4; ++i)
	{
		FSlot& S = Slots[i];
		if (!S.bActive) { continue; }

		S.Age += DeltaTime;
		const float A = FMath::Clamp(S.Age / S.Duration, 0.f, 1.f);
		S.Radius = FMath::Lerp(0.f, S.MaxR, A);
		Strength[i] = (1.f - A * A) * S.Peak;

		if (A >= 1.f)
		{
			S.bActive = false;
			Strength[i] = 0.f;
		}
	}

	WriteToMPC(Strength);
}




