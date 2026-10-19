// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
private:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAISenseConfig_Hearing* HearingConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	class UStateTreeAIComponent* StateTreeComponent;
	
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void TargetForgotten(AActor* ForgottenActor);
	const UObject* GetCurrentTarget() const;

	AActor* GetNextPerceivedActor() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Target")
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Target")
	TObjectPtr<AActor> CurrentTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Target")
	bool bHasTarget = false;
	
};
