// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"
class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;
UCLASS()

class THIRDPERSONRPG_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PickUp)
	USphereComponent* sphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UStaticMeshComponent* staticComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	UParticleSystemComponent* particlesysComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	UParticleSystem* particleSys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* souCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	bool bIsRotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	float RotateRate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&SweepResult);
	UFUNCTION()
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
