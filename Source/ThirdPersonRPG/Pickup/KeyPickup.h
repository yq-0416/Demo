// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "KeyPickup.generated.h"
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class THIRDPERSONRPG_API AKeyPickup : public APickupBase
{
	GENERATED_BODY()
	AKeyPickup();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup)
	UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pickup)
	TSubclassOf<UUserWidget>KeyWidgetAsset;
	
public:
	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

protected:
	virtual void BeginPlay() override;
};
