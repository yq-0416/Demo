// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONRPG_API AHealthPickup : public APickupBase
{
	GENERATED_BODY()
	void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

public:
	AHealthPickup();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PickUp)
	float healthAmount;

};
