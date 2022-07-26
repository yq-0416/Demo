// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HintTriggerVol.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UBillboardComponent;
class UWidgetComponent;
class USoundCue;









UCLASS()
class THIRDPERSONRPG_API AHintTriggerVol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHintTriggerVol();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HintTrigger)
	UBoxComponent* boxComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HintTrigger)
	UStaticMeshComponent* staticComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HintTrigger)
	UBillboardComponent* billComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HintTrigger)
	UWidgetComponent* widComp;
	FORCEINLINE UWidgetComponent* GetWidComp() { return widComp; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* souCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	TSubclassOf<UUserWidget>hintWidgetAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnHintTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnHintTriggerlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
