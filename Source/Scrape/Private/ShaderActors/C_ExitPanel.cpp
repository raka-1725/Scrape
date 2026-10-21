// Fill out your copyright notice in the Description page of Project Settings.


#include "ShaderActors/C_ExitPanel.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AC_ExitPanel::AC_ExitPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetRenderCustomDepth(true); //mask
	Mesh->SetCustomDepthStencilValue(StencilValue);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);
	Trigger->SetBoxExtent(FVector(80.f, 120.f, 130.f));
	Trigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AC_ExitPanel::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetCustomDepthStencilValue(StencilValue);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AC_ExitPanel::OnTriggerBegin);
}

void AC_ExitPanel::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bEscaped) { return; }

	const ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (Char && Char->IsPlayerControlled())
	{
		Win();
	}
}

void AC_ExitPanel::Win()
{
	bEscaped = true;
	UE_LOG(LogTemp, Warning, TEXT("Escape"));
	OnPlayerEscaped.Broadcast();
	
	//implement win sequence
}


