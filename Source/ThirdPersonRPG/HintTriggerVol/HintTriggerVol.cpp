// Fill out your copyright notice in the Description page of Project Settings.


#include "HintTriggerVol.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include"Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"


// Sets default values
AHintTriggerVol::AHintTriggerVol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;

	staticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticComp"));
	staticComp->SetupAttachment(RootComponent);

	billComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("billComp"));
	billComp->SetupAttachment(RootComponent);

	widComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widComp"));
	widComp->SetupAttachment(RootComponent);

	widComp->SetPivot(FVector2D(0.0f,0.0f));
	widComp->SetVisibility(false);
	widComp->SetWidgetSpace(EWidgetSpace::Screen);
	souCue = nullptr;

}

// Called when the game starts or when spawned
void AHintTriggerVol::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AHintTriggerVol::OnHintTriggerOverlapBegin);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AHintTriggerVol::OnHintTriggerlapEnd);
	if (hintWidgetAsset)
	{
		widComp->SetWidgetClass(hintWidgetAsset);
	}
}

// Called every frame
void AHintTriggerVol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHintTriggerVol::OnHintTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (souCue)
			{
				UGameplayStatics::PlaySound2D(this, souCue);
			}
		}
	}
}

void AHintTriggerVol::OnHintTriggerlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			
		}
	}
}

