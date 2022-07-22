// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HintTriggerVol.h"
#include "GameVectoryVol.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONRPG_API AGameVectoryVol : public AHintTriggerVol
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintImplementableEvent)
	void OpenVictoryDoor();

	virtual void OnHintTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void OnHintTriggerlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
