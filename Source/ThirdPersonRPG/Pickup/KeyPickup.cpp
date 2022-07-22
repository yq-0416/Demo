// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"
#include "Components/WidgetComponent.h"

AKeyPickup::AKeyPickup()
{
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(GetRootComponent());

	widgetComp->SetVisibility(false);
	widgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void AKeyPickup::BeginPlay()
{
	Super::BeginPlay();
	if (KeyWidgetAsset)
	{
		widgetComp->SetWidgetClass(KeyWidgetAsset);
	}
	if (KeyWidgetAsset)
	{
		widgetComp->SetVisibility(true);
		UE_LOG(LogTemp, Warning, TEXT("ACoinPickup::I am in"));
	}
}

void AKeyPickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			mainPlayer->SetKeyFunc();
			Destroy();
		}
	}
}
void AKeyPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


