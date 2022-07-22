// Fill out your copyright notice in the Description page of Project Settings.


#include "GameVectoryVol.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"
#include"Components/WidgetComponent.h"

void AGameVectoryVol::OnHintTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnHintTriggerOverlapBegin(OverlappedComponent,OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (mainPlayer->GetHasKey())
			{
				OpenVictoryDoor();
				mainPlayer->GameWin();
			}
			else
			{
				GetWidComp()->SetVisibility(true);
			}
		}
		
	}
}

void AGameVectoryVol::OnHintTriggerlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnHintTriggerlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (mainPlayer->GetHasKey())
			{
				//OpenVictoryDoor();
			}
			else
			{
				GetWidComp()->SetVisibility(false);
			}
		}
		
	}
}
