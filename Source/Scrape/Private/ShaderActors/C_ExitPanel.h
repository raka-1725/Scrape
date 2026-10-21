// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_ExitPanel.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEscaped);

UCLASS()
class AC_ExitPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ExitPanel();
	
	UPROPERTY(BlueprintAssignable) 
	FOnPlayerEscaped OnPlayerEscaped;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	
	void Win();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere) 
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = "Exit") 
	int32 StencilValue = 1;
	
	UPROPERTY(EditAnywhere, Category = "Exit") 
	float FadeTime = 2.0f;
	
private:
	bool bEscaped = false;
	FTimerHandle ReloadTimer;

};
