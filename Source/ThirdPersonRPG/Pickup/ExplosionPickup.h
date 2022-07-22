// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "ExplosionPickup.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONRPG_API AExplosionPickup : public APickupBase
{
	GENERATED_BODY()
public:
	AExplosionPickup();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = PickUp)
	float damageAmount;
	void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
};
