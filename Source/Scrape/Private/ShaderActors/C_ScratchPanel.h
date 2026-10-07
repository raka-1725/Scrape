// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_ScratchPanel.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTextureRenderTarget2D;

UCLASS()
class AC_ScratchPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ScratchPanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
private:
	static const int32 NumHoles = 4;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> M_Wall;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> M_Scratch;

	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> MID_Wall;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> MID_Brush;

	UPROPERTY(EditAnywhere, Category = "Panel")
	FVector2D WallSize = FVector2D(200.0, 250.0);   // cm

	UPROPERTY(EditAnywhere, Category = "Panel")
	FVector2D SeedUV = FVector2D(0.5, 0.5);

	UPROPERTY(EditAnywhere, Category = "Panel")
	float Seed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float SeedRadius = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float GrowSPD = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float ShrinkSPD = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float MaxRadius = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float PassRadius = 55.0f;

	UPROPERTY(EditAnywhere, Category = "Scratch")
	float MergeDist = 15.0f;
	
	FVector2D HoleUV[NumHoles];
	
	float HoleR[NumHoles] = {};
	bool bScratchedThisFrame[NumHoles] = {};
	
	bool bOpen = false;
	bool bDirty = false;
	
public:
	UFUNCTION()
	float ApplyScratch(const FVector2D& UV, float DeltaTime);
	
	bool IsOpen() const { return bOpen; }
private:
	UFUNCTION()
	UTextureRenderTarget2D* CreareRenderTarget();
	
	UFUNCTION()
	void ReDraw();
	
	void UpdateOpenState();
};
