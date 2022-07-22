// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */

class USkeletalMeshComponent;
class UWidgetComponent;
class UBoxComponent;
class AMainPlayer;
class UParticleSystem;
class USoundCue;
class AController;





UCLASS()
class THIRDPERSONRPG_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()
	
public:

	AWeaponPickup();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Pickup)
	USkeletalMeshComponent* skeMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup)
	UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pickup)
	TSubclassOf<UUserWidget>weaponHintWidgetAsset;

	//Damage of weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup)
	UBoxComponent* attackBoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	float weaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	USoundCue* swingCue;

	

	UFUNCTION(BlueprintCallable)
	void NotifyAnim_ActiveWeaponCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeacWeaponCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_WeaponPlayWing();

	void EquipWeapon(AMainPlayer* mainPlayer);

	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//武器伤害敌人
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	TSubclassOf<class UDamageType> damType;
	AController* weaponInstigator;
	FORCEINLINE void SetWeaponInstigator(AController* weaponIns) { weaponInstigator = weaponIns; }

protected:
	virtual void BeginPlay() override;


};
