// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "ThirdPersonRPG/Enermy/MyEnermy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound\SoundCue.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"


AWeaponPickup::AWeaponPickup()
{
	skeMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeMeshComp"));
	skeMeshComp->SetupAttachment(GetRootComponent());

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(GetRootComponent());

	attackBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("attackBoxComp"));
	attackBoxComp->SetupAttachment(GetRootComponent());

	//particleSys = nullptr;
	//souCue = nullptr;

	//角色攻击敌人
	weaponDamage = 10.0f;
	swingCue = nullptr;

	widgetComp->SetVisibility(false);
	widgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	attackBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnAttackOverlapBegin);
	attackBoxComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponPickup::OnAttackOverlapEnd);

	//设置武器的碰撞
	attackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	attackBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	attackBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (weaponHintWidgetAsset)
	{
		widgetComp->SetWidgetClass(weaponHintWidgetAsset);
	}
}


void AWeaponPickup::EquipWeapon(AMainPlayer* mainPlayer)
{
	const USkeletalMeshSocket* meshSocket = mainPlayer->GetMesh()->GetSocketByName("RightHandSocket");
	if (meshSocket)
	{
		weaponInstigator = mainPlayer->GetController();
		meshSocket->AttachActor(this, mainPlayer->GetMesh());
		mainPlayer->SetWeaponHold(this);
		mainPlayer->SetPickupOverlap(nullptr);
		bIsRotate = false;
		particlesysComp->Deactivate();
	}
	if (weaponHintWidgetAsset)
	{
		widgetComp->SetVisibility(false);
	}
}

void AWeaponPickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPickupOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* mainplayer = Cast<AMainPlayer>(OtherActor);
		if (mainplayer)
		{
			
			if (weaponHintWidgetAsset)
			{
				widgetComp->SetVisibility(true);
			}
			
			mainplayer->SetPickupOverlap(this);
			
		}
		
	}
}

void AWeaponPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainPlayer* mainplayer = Cast<AMainPlayer>(OtherActor);
		if (mainplayer)
		{
			if (weaponHintWidgetAsset)
			{
				widgetComp->SetVisibility(false);
			}
			mainplayer->SetPickupOverlap(nullptr);
		}
		
	}
}

void AWeaponPickup::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMyEnermy* enemy = Cast<AMyEnermy>(OtherActor);
		if (enemy)
		{
			//血液飞溅效果
			if (enemy->hitParticle)
			{
				const USkeletalMeshSocket* weaponSocket = skeMeshComp->GetSocketByName("weaponSocket");
				UGameplayStatics::SpawnEmitterAtLocation(this, enemy->hitParticle, weaponSocket->GetSocketLocation(skeMeshComp), FRotator(0.0f), true);
			}
			//打击配乐
			if (enemy->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, enemy->hitSound);
			}
			if (damType)
			{
				UGameplayStatics::ApplyDamage(enemy, weaponDamage, weaponInstigator, this, damType);
			}
		}
		
	}
}

void AWeaponPickup::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMyEnermy* enermy = Cast<AMyEnermy>(OtherActor);
	}

}


void AWeaponPickup::NotifyAnim_ActiveWeaponCollison()
{
	attackBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponPickup::NotifyAnim_DeacWeaponCollison()
{
	attackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponPickup::NotifyAnim_WeaponPlayWing()
{
	if (swingCue)
	{
		UGameplayStatics::PlaySound2D(this,swingCue);
	}
}

