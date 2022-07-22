// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"
//#include "Components/SceneComponent.h"

// Sets default values 
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	RootComponent = sphereComp;

	staticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticComp"));
	staticComp->SetupAttachment(RootComponent);

	particlesysComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particlesysComp"));
	particlesysComp->SetupAttachment(RootComponent);

	particleSys = nullptr;
	souCue = nullptr;

	bIsRotate = false;
	RotateRate = 45;

}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this,&APickupBase::OnPickupOverlapBegin);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &APickupBase::OnPickupOverlapEnd);

}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsRotate)
	{
		FRotator rot = GetActorRotation();
		rot.Yaw += DeltaTime * RotateRate;
		SetActorRotation(rot);
	}

}

void APickupBase::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Super::I am in"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (particleSys)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, particleSys, GetActorLocation(), FRotator(0.0f), true);
			}
			if (souCue)
				{
					UGameplayStatics::PlaySound2D(this, souCue);
				}
		}
	}
	
	
}

void APickupBase::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Super::I am out"));
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			
		}
	}
}

