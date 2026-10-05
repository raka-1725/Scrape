// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ScratchPanel.h"

#include "VectorTypes.h"
#include "Engine/TextureRenderTarget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Splines/SplineMath.h"

// Sets default values
AC_ScratchPanel::AC_ScratchPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AC_ScratchPanel::BeginPlay()
{
	Super::BeginPlay();
	RenderTarget = CreareRenderTarget();
	MID_Wall = Mesh->CreateDynamicMaterialInstance(0, M_Wall);
	MID_Brush = UMaterialInstanceDynamic::Create(M_Scratch, this);
	
	MID_Wall->SetTextureParameterValue("ScratchTex",RenderTarget);
	MID_Wall->SetVectorParameterValue("WallSize", FLinearColor(WallSize.X, WallSize.Y, 0,0));
	MID_Brush->SetVectorParameterValue("WallSize", FLinearColor(WallSize.X, WallSize.Y, 0,0));
	MID_Wall->SetScalarParameterValue("Seed", Seed);
	MID_Brush->SetScalarParameterValue("Seed", Seed);
	Mesh->SetMaterial(0, MID_Wall);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor::Black);
}

// Called every frame
void AC_ScratchPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTextureRenderTarget2D* AC_ScratchPanel::CreareRenderTarget()
{
	UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>(this);
	
	RT->RenderTargetFormat = RTF_R16f;
	RT->InitCustomFormat(512,512,PF_R16F, false);
	RT->ClearColor = FLinearColor::Black;
	RT->UpdateResourceImmediate(true);
	
	return RT;
}

float AC_ScratchPanel::ApplyScratch(const FVector2D& UV, float DeltaTime)
{
	float dist = ((UV - SeedUV) * WallSize).Size();
	if (dist > SeedRadius)
	{
		return 0.5f;
	}
	
	
}

