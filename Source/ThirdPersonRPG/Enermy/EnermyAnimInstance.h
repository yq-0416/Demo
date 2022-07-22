// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnermyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONRPG_API UEnermyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	float MoovSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement)
	APawn* pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = movement)
	class AMyEnermy* enermy;

protected:
	virtual void NativeInitializeAnimation()override;
	UFUNCTION(BlueprintCallable, Category = AnimProperty)
	void UpdateAnimProperties();
};
