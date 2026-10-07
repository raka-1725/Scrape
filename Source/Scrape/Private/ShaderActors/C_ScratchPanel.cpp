// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ScratchPanel.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "opencv2/core/types.hpp"

// Sets default values
AC_ScratchPanel::AC_ScratchPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
}

// Called when the game starts or when spawned
void AC_ScratchPanel::BeginPlay()
{
	Super::BeginPlay();
	
	RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
			this, 512, 512, ETextureRenderTargetFormat::RTF_R16f,
			FLinearColor::Black, false);

	MID_Wall  = Mesh->CreateDynamicMaterialInstance(0, M_Wall);
	MID_Brush = UMaterialInstanceDynamic::Create(M_Scratch, this);

	const FLinearColor Size(WallSize.X, WallSize.Y, 0.f, 0.f);

	MID_Wall->SetTextureParameterValue(TEXT("ScratchTex"), RenderTarget);
	MID_Wall->SetVectorParameterValue(TEXT("WallSize"), Size);
	MID_Wall->SetScalarParameterValue(TEXT("Seed"), Seed);

	MID_Brush->SetVectorParameterValue(TEXT("WallSize"), Size);
	MID_Brush->SetScalarParameterValue(TEXT("Seed"), Seed);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor::Black);
}

// Called every frame
void AC_ScratchPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (int32 i = 0; i < NumHoles; ++i)
	{
		if (HoleR[i] > 0.f)
		{
			if (!bScratchedThisFrame[i])
			{
				HoleR[i] = FMath::Max(0.f, HoleR[i] - ShrinkSPD * DeltaTime);
			}
			bScratchedThisFrame[i] = false;
			bDirty = true;
		}
	}

	if (bDirty)
	{
		ReDraw();
		UpdateOpenState();
		bDirty = false;
	}
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
	if (((UV - SeedUV) * WallSize).Size() > SeedRadius)
	{
		return 0.5f;
	}
	
	int32 Found = INDEX_NONE;
	int32 Empty = INDEX_NONE;

	for (int32 i = 0; i < NumHoles; ++i)
	{
		if (HoleR[i] <= 0.f)
		{
			if (Empty == INDEX_NONE) { Empty = i; }
			continue;
		}
		if (((UV - HoleUV[i]) * WallSize).Size() < MergeDist)
		{
			Found = i;
			break;
		}
	}
	
	if (Found != INDEX_NONE)
	{
		//add nosie here
		HoleR[Found] = FMath::Min(HoleR[Found] + GrowSPD * DeltaTime, MaxRadius);
		bScratchedThisFrame[Found] = true;
	}
	else if (Empty != INDEX_NONE)
	{
		HoleUV[Empty] = UV;
		HoleR[Empty]  = 5.f;
		bScratchedThisFrame[Empty] = true;
	}

	bDirty = true;
	return 1.0f;
}

void AC_ScratchPanel::ReDraw()
{
	static const FName HoleNames[NumHoles] = {
		FName(TEXT("Hole0")), FName(TEXT("Hole1")),
		FName(TEXT("Hole2")), FName(TEXT("Hole3")) };

	for (int32 i = 0; i < NumHoles; ++i)
	{
		MID_Brush->SetVectorParameterValue(HoleNames[i],
			FLinearColor(HoleUV[i].X, HoleUV[i].Y, HoleR[i], 0.f));
	}

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor::Black);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RenderTarget, MID_Brush);
}

void AC_ScratchPanel::UpdateOpenState()
{
	bool bNow = false;
	for (int32 i = 0; i < NumHoles; ++i)
	{
		if (HoleR[i] >= PassRadius) { bNow = true; break; }
	}
	if (bNow == bOpen) { return; }
	bOpen = bNow;
	
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, bOpen ? ECR_Ignore : ECR_Block);
}

