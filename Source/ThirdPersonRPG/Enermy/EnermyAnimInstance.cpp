// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ThirdPersonRPG/Enermy/MyEnermy.h"

void UEnermyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			enermy = Cast<AMyEnermy>(pawn);
		}
	}
}

void UEnermyAnimInstance::UpdateAnimProperties()
{
	if (!pawn)
	{
		pawn = TryGetPawnOwner();
	}
	if (pawn)
	{
		if (!enermy)
		{
			enermy = Cast<AMyEnermy>(pawn);
		}

		FVector playerVel = pawn->GetVelocity();
		FVector playerVel2 = FVector(playerVel.X, playerVel.Y, 0.0f);

		MoovSpeed = playerVel2.Size();

	}
}