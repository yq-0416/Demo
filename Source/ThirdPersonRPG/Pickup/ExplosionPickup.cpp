// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionPickup.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"

AExplosionPickup::AExplosionPickup()
{
	damageAmount = 5.0f;
}

void AExplosionPickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("ACoinPickup::I am in"));
	AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
	if (mainPlayer)
	{
		mainPlayer->DecreaseHealth(damageAmount);
		Destroy();
	}
}

void AExplosionPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("ACoinPickup::I am out"));
}