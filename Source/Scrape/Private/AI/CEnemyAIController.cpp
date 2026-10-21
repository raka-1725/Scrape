// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CEnemyAIController.h"

#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

ACEnemyAIController::ACEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");
	
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->SetMaxAge(5.0f);
	
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->SetDominantSense(HearingConfig->GetSenseImplementation());
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	SetPerceptionComponent(*AIPerceptionComponent);
	
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeComponent");
}

void ACEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UE_LOG(LogTemp, Warning,
	TEXT("AI possessed Pawn: %s"),
	*GetNameSafe(InPawn));
}

void ACEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACEnemyAIController::TargetForgotten);
	
	UE_LOG(LogTemp, Warning,
	TEXT("Enemy AIController BeginPlay: %s"),
	*GetName());
}

void ACEnemyAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning,TEXT("Perception: Actor=%s, Sensed=%d, StimulusLocation=%s"),*GetNameSafe(TargetActor),Stimulus.WasSuccessfullySensed(),*Stimulus.StimulusLocation.ToString());

	if (!Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	CurrentTarget = TargetActor;
	bHasTarget = true;
	TargetLocation = Stimulus.StimulusLocation;
}

void ACEnemyAIController::TargetForgotten(AActor* ForgottenActor)
{
	if (!IsValid(ForgottenActor) || CurrentTarget != ForgottenActor)
	{
		return;
	}

	CurrentTarget = GetNextPerceivedActor();

	if (IsValid(CurrentTarget))
	{
		TargetLocation = CurrentTarget->GetActorLocation();
		bHasTarget = true;
	}
	else
	{
		bHasTarget = false;
	}
}

const UObject* ACEnemyAIController::GetCurrentTarget() const
{
	return CurrentTarget;
}
AActor* ACEnemyAIController::GetNextPerceivedActor() const
{
	if (PerceptionComponent)
	{
		TArray<AActor*> Actors;
		AIPerceptionComponent->GetPerceivedHostileActors(Actors);
		if (Actors.Num() != 0)
		{
			return Actors[0];
		}
	}
	return nullptr;
}