// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_ScratchPanel.generated.h"

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
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr< UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, Category = "RenderTarget")
	class UTextureRenderTarget2D* RenderTarget;
	
	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> M_Wall;
	
	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> M_Scratch;
	
	UPROPERTY(EditAnywhere, Category = "Material | DynamicInstance")
	UMaterialInstanceDynamic* MID_Wall;
	
	UPROPERTY(EditAnywhere, Category = "Material | DynamicInstance")
	UMaterialInstanceDynamic* MID_Brush;
	
	UPROPERTY(EditAnywhere, Category = "Hole | UV")
	TArray<FVector2D> Hole;
		
	UPROPERTY(EditAnywhere, Category = "Hole | Radius")
	TArray<float> HoleR;
	
	UPROPERTY(EditAnywhere, Category = "WallSize")
	FVector2D WallSize;
	
	UPROPERTY(EditAnywhere, Category = "Seed")
	FVector2D SeedUV;
	
	UPROPERTY(EditAnywhere, Category = "Seed")
	float Seed;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float SeedRadius = 25.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float GrowSPD = 25.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float ShrinkSPD = 4.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float MaxRadius = 90.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float PassRadius = 55.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	float MergeDist = 15.0f;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	TArray<bool> bScratchedThisFrame;
	
	UPROPERTY(EditAnywhere, Category = "Scratch | Settings")
	bool bOpen = false;
	
	
private:
	UFUNCTION()
	UTextureRenderTarget2D* CreareRenderTarget();
	
	UFUNCTION()
	float ApplyScratch(const FVector2D& UV, float DeltaTime);
};
