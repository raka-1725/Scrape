// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ScratchToolComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShaderActors/C_ScratchPanel.h"

class AC_ScratchPanel;
// Sets default values for this component's properties
UC_ScratchToolComponent::UC_ScratchToolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UC_ScratchToolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	
	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
	{
		if (bScratching && !bWasScratching)
		{
			CachedWalkSpeed = Move->MaxWalkSpeed;
			Move->MaxWalkSpeed = CachedWalkSpeed * ScratchMoveSpeedScale;
		}
		else if (!bScratching && bWasScratching)
		{
			Move->MaxWalkSpeed = CachedWalkSpeed;
		}
	}
	bWasScratching = bScratching;
	if (!bScratching) return;

	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC || !PC->PlayerCameraManager) return;

	const FVector Start = PC->PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + PC->PlayerCameraManager->GetActorForwardVector() * Reach;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ScratchTrace),true, Character);
	Params.bReturnFaceIndex = true;

	FHitResult Hit;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		return;

	AC_ScratchPanel* Panel = Cast<AC_ScratchPanel>(Hit.GetActor());
	if (!Panel) return;

	FVector2D UV;
	if (!UGameplayStatics::FindCollisionUV(Hit, 0, UV)) return;

	const float Factor = Panel->ApplyScratch(UV, DeltaTime);
	
}

