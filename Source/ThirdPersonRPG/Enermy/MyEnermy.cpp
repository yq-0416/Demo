// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnermy.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayer.h"
#include "AIModule/Classes/AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ThirdPersonRPG/Pickup/WeaponPickup.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayerController.h"


// Sets default values
AMyEnermy::AMyEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	aproSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("aproSphereComp"));
	aproSphereComp->SetupAttachment(RootComponent);
	aproSphereComp->InitSphereRadius(600.0f);

	attackSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("attackSphereComp"));
	attackSphereComp->SetupAttachment(RootComponent);
	attackSphereComp->InitSphereRadius(75.0f);

	LegComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LegComp"));
	LegComp->SetupAttachment(GetMesh(), FName("enermySocket"));
	enermyState = EEnermyState::ES_IDLE;

	bInAgroSphere = false;
	bInAttackSphere = false;
	bAttacking = false;
	attackMontage = nullptr;
	playerTarget = nullptr;

	acuTime = 0.0f;
	tickChaseTime = 2.0f;

	//与玩家进行打斗
	maxHealth = 100.0f;
	health = maxHealth;
	enermyDamage = 2.0f;
	hitParticle = nullptr;
	hitSound = nullptr;
	swingSound = nullptr;

	//敌人延迟进攻
	minAttackTime = 0.5f;
	maxAttackTime = 3.5f;

	deathDelay = 2.0f;
	bHasVaileTarget = false;

	//敌人漫游
	bCanWander = false;
	bWandering = true;
	timePause = 2.0f;
	timeAccumWander = 0.0f;
}


void AMyEnermy::FindSphereRandomLocation(FVector& randLocation)
{
	FVector enermyLocation = GetActorLocation();
	float radius = FMath::RandRange(200.0f, 1000.0f);
	float angle = FMath::FRandRange(0.0f, 360.0f);
	float deltaX = radius * FMath::Cos(FMath::DegreesToRadians(angle));
	float deltaY = radius * FMath::Sin(FMath::DegreesToRadians(angle));
	randLocation = FVector(enermyLocation.X + deltaX, enermyLocation.Y + deltaY, enermyLocation.Z);
}

// Called when the game starts or when spawned
void AMyEnermy::BeginPlay()
{
	Super::BeginPlay();
	enermyController = Cast<AAIController>(GetController());
	aproSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyEnermy::OnAproSphereOverlapBegin);
	aproSphereComp->OnComponentEndOverlap.AddDynamic(this, &AMyEnermy::OnAproSphereOverlapEnd);
	attackSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyEnermy::OnAttackSphereOverlapBegin);
	attackSphereComp->OnComponentEndOverlap.AddDynamic(this, &AMyEnermy::OnAttackSphereOverlapEnd);

	LegComp->OnComponentBeginOverlap.AddDynamic(this, &AMyEnermy::OnAttackOverlapBegin);
	LegComp->OnComponentEndOverlap.AddDynamic(this, &AMyEnermy::OnAttackOverlapEnd);

	LegComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LegComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LegComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LegComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//设置跟相机的碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//敌人漫游随机种子
	double timeMs = FTimespan(FDateTime::Now().GetTicks()).GetTotalMilliseconds();
	int32 timeSeed = (int32)(((int64)timeMs) % INT_MAX);
	FMath::RandInit(timeSeed);
	timePause = FMath::RandRange(2.0f, 4.0f);
}

// Called every frame
void AMyEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	acuTime += DeltaTime;
	if (acuTime >= tickChaseTime)
	{
		acuTime = 0.0f;
		if (enermyController && EnermyAlive())
		{
			if (playerTarget)
			{
				if (bInAgroSphere && !bInAttackSphere && !bAttacking && enermyState == EEnermyState::ES_MoveToTarget && GetCharacterMovement()->Velocity.Size() < 10.0f)
				{

					MoveToTarget(playerTarget);
				}
			}
		}
	}
	timeAccumWander += DeltaTime;
	if (bCanWander&& EnermyAlive())
	{
		if (bWandering && timeAccumWander > timePause && !bInAgroSphere && !bInAttackSphere)
		{
			SetEnermyState(EEnermyState::ES_IDLE);
			if (enermyController)
			{
				//UE_LOG(LogTemp, Warning, TEXT("AMyEnermy::wanding"));
				FVector rdLocation;
				FindSphereRandomLocation(rdLocation);
				enermyController->MoveToLocation(rdLocation, 4.0f);
			}
			timeAccumWander = 0.0f;
			timePause = FMath::RandRange(2.0f, 4.0f);
		}
	}

}

// Called to bind functionality to input
void AMyEnermy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnermy::OnAproSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			bHasVaileTarget = true;
			if (!(main->PlayerAlive())) return;
			main->SetHasEnermyTarget(true);
			main->SetTargetEnermy(this);
			playerTarget = main;
			bWandering = false;
			bInAgroSphere = true;
			MoveToTarget(main);
		}
	}
}

void AMyEnermy::OnAproSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			bHasVaileTarget = false;
			if (!(main->PlayerAlive())) return;
			main->SetHasEnermyTarget(false);
			main->SetTargetEnermy(nullptr);
			bInAgroSphere = false;
			playerTarget = nullptr;
			bWandering = true;
			if (!bCanWander)
			{
				if (enermyController)
					{
						
						SetEnermyState(EEnermyState::ES_IDLE);
						enermyController->StopMovement();				
					}
			}
			
			
		}
	}
}

void AMyEnermy::OnAttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			if (!(main->PlayerAlive())) return;
			main->SetHasEnermyTarget(true);
			main->SetTargetEnermy(this);
			if (main->mainPlayerControler)
			{
				main->mainPlayerControler->DisplayEnermyHealthBar();
			}
			//bInAttackSphere = true;
			Enermy_Attack();
		}
	}
}

void AMyEnermy::OnAttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* main = Cast<AMainPlayer>(OtherActor);
		if (main)
		{
			if (!(main->PlayerAlive())) return;
			if (main->enermyTarget == this)
			{
				main->SetTargetEnermy(nullptr);
			}
			if (main->mainPlayerControler)
			{
				main->mainPlayerControler->RemoveEnermyHealthBar();
			}
			bInAttackSphere = false;
			MoveToTarget(main);
			
		}
		GetWorldTimerManager().ClearTimer(attackTimer);
	}
}

void AMyEnermy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AMyEnermy::hit"));
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		//血液飞溅效果
		
		if (mainPlayer) 
		{
			if (!(mainPlayer->PlayerAlive())) return;
			if (mainPlayer->hitParticle)
			{
				
				const USkeletalMeshSocket* tipSocket = GetMesh()->GetSocketByName("tipSocket");
				UGameplayStatics::SpawnEmitterAtLocation(this, mainPlayer->hitParticle, tipSocket->GetSocketLocation(GetMesh()), FRotator(0.0f), true);
			}
			//打击配乐
			if (mainPlayer->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, mainPlayer->hitSound);
			}
			if (damType)
			{
				UGameplayStatics::ApplyDamage(mainPlayer, enermyDamage, enermyController, this, damType);
			}
		}		
	}
}

void AMyEnermy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && EnermyAlive())
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (!(mainPlayer->PlayerAlive())) return;
		}
	}
}

void AMyEnermy::MoveToTarget(AMainPlayer* target)
{
	if (enermyController)
	{
		SetEnermyState(EEnermyState::ES_MoveToTarget);
		FAIMoveRequest request;
		request.SetGoalActor(target);
		request.SetAcceptanceRadius(20.0f);
		FNavPathSharedPtr out;
		enermyController->MoveTo(request, &out);
	}
}



void AMyEnermy::NotifAnim_AttackEnd()
{
	bAttacking = false;
	if (bInAttackSphere && EnermyAlive() && bHasVaileTarget)
	{
		bInAttackSphere = false;
		float attackRandTime = FMath:: FRandRange(minAttackTime, maxAttackTime);
		GetWorldTimerManager().SetTimer(attackTimer,this, &AMyEnermy::Enermy_Attack, attackRandTime);
		//Enermy_Attack();
	}
}

void AMyEnermy::Enermy_Attack()
{
	if (enermyController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AMyEnermy::Attack_1"));
		enermyController->StopMovement();
		SetEnermyState(EEnermyState::ES_ATTACK);
	}
	if (!bInAttackSphere &&EnermyAlive() && bHasVaileTarget)
	{
		bInAttackSphere = true;
		UAnimInstance* animInst = GetMesh()->GetAnimInstance();
		if (animInst && attackMontage)
		{
			animInst->Montage_Play(attackMontage);
			animInst->Montage_JumpToSection(FName("Attack"), attackMontage);
		}
	}
}

void AMyEnermy::NotifyAnim_ActiveEnermyCollison()
{
	LegComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMyEnermy::NotifyAnim_DeacEnermyCollison()
{
	LegComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyEnermy::NotifyAnim_EnermyPlayWing()
{

	if (swingSound)
	{
		UGameplayStatics::PlaySound2D(this, swingSound);
	}
}


void AMyEnermy::Die()
{
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst && attackMontage)
	{
		animInst->Montage_Play(attackMontage);
		animInst->Montage_JumpToSection("Death", attackMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LegComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	aproSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


float AMyEnermy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (health - Damage <= 0.0f)
	{
		health = 0.0f;
		SetEnermyState(EEnermyState::ES_DEAD);
		if (EventInstigator)
		{
			AMainPlayerController* mainController = Cast<AMainPlayerController>(EventInstigator);
			if (mainController)
			{
				mainController->RemoveEnermyHealthBar();
			}
		}
		if (DamageCauser)
		{
			AMyEnermy* enermy = Cast<AMyEnermy>(DamageCauser);
			if (enermy)
			{
				enermy->bHasVaileTarget = false;
			}
		}
		Die();
	}
	else
	{
		health -= Damage;
	}
	return Damage;
}

void AMyEnermy::NotifyAnim_DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(deathTime,this,&AMyEnermy::DestoryCalled,deathDelay);
}

void AMyEnermy::DestoryCalled()
{
}

bool AMyEnermy::EnermyAlive()
{
	return enermyState != EEnermyState::ES_DEAD;
}
