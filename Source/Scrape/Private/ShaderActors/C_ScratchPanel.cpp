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
	
	bool bPlayerInside = false;
	
	for (int32 i = 0; i < 4; ++i)
	{
		if (HoleR[i] > 0.f)
		{
			if (!bScratchedThisFrame[i] && !bPlayerInside)
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
	float dist = ((UV - SeedUV) * WallSize).Size();
	if (dist > SeedRadius)
	{
		return 0.5f;
	}

	for (int i = 0; i < Hole.GetAllocatedSize(); ++i)
	{
		if (((UV - Hole[i]) * WallSize).Size() < MergeDist)
		{
			HoleR[i] = FMath::Min(HoleR[i] + GrowSPD * DeltaTime, MaxRadius);
			bScratchedThisFrame[i] = true;
		}
		else
		{
			for (int j = 0; HoleR.GetAllocatedSize(); ++j)
			{
				if (HoleR[j] == 0)
				{
					Hole[j] = UV, HoleR[j] = 5;
					bScratchedThisFrame[j] = true;
				}
			}
		}
	}
	return 1.0f;
}

void AC_ScratchPanel::ReDraw()
{
	for (int32 i = 0; i < 4; ++i)
	{
		const FName Name(*FString::Printf(TEXT("Hole%d"), i));
		MID_Brush->SetVectorParameterValue(Name,
			FLinearColor(Hole[i].X, Hole[i].Y, HoleR[i], 0.f));
	}
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, FLinearColor::Black);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RenderTarget, MID_Brush);
}

