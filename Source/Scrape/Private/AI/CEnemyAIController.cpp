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
	
	HearingConfig->HearingRange = 1200.0f;
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeComponent");
}

void ACEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACEnemyAIController::BeginPlay()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACEnemyAIController::TargetForgotten);
}

void ACEnemyAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
}

void ACEnemyAIController::TargetForgotten(AActor* ForgottenActor)
{
}
