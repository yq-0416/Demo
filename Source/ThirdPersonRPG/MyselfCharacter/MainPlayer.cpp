// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "ThirdPersonRPG/Pickup/WeaponPickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "ThirdPersonRPG/Enermy/MyEnermy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "ThirdPersonRPG/MyselfCharacter/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mainPlayerState = EMainPlayerState::MPS_NORMAL;
	MaxHealth = 100.0f;
	Health = MaxHealth;
	CoinCount = 0;

	//加速跑
	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	RunSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintLoss = 5.0f;
	SprintSpeed = RunSpeed + 300.0f;
	bForwardMove = false;
	bRightMove = false;
	bRMouthdown = false;
	
	//武器初始值
	bLMBDown = false;
	weaponHold = nullptr;
	pickupOverlap = nullptr;

	//攻击动画
	bAttacking = false;
	attackMontage = nullptr;
	hitSound = nullptr;
	hitParticle = nullptr;

	interpSpeed = 10.0f;
	bInterpToEnermy = false;
	bHasEnermyTarget = false;

	//游戏胜利条件
	allCoinNum = 0;
	bHasKey = false;
	keyNum = 0;
	targetLocation = FVector(1080.0f, 4680.0f, -280.0f);

	//游戏胜利或失败相关初始值
	bHasGameWin = false;
	winSound = nullptr;
	loseSound = nullptr;
}

void AMainPlayer::LMouseDown()
{
	bLMBDown = false;
}

void AMainPlayer::LMouseUp()
{
	bLMBDown = true;
	if (pickupOverlap)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AMainPlayer::Attack_1"));
		AWeaponPickup* weapon = Cast<AWeaponPickup>(pickupOverlap);
		weapon->EquipWeapon(this);
	}
	else if (weaponHold)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AMainPlayer::Attack_2"));
		Attack();
	}

}

void AMainPlayer::Attack()
{
	//UE_LOG(LogTemp, Warning, TEXT("AMainPlayer::Attack"));
	if (!PlayerAlive()) return;//&&(!bHasGameWin)
	if (!bAttacking)
	{
		bAttacking = true;
		SetInterpToEnermy(true);
			UAnimInstance* animInst = GetMesh()->GetAnimInstance();
			if (animInst && attackMontage)
			{
				int32 section = FMath::RandRange(0, 1);
				switch (section)
				{
				case 0:
					animInst->Montage_Play(attackMontage);
					animInst->Montage_JumpToSection("Attack_1", attackMontage);
					
					break;
				case 1:
					animInst->Montage_Play(attackMontage);
					animInst->Montage_JumpToSection("Attack_2", attackMontage);
					
					break;
				default:
					break;
				}
			}
	}
	
}

void AMainPlayer::NotifyAnim_AttackingEnding()
{
	if (!PlayerAlive()) return;
	bAttacking = false;
	SetInterpToEnermy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

FRotator AMainPlayer::GetLookAtRotationYaw(FVector targetVector)
{
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetVector);
	return rot;
}



void AMainPlayer::SetKeyFunc()
{
	keyNum += 1;
	SetHasKey(true);
}

void AMainPlayer::SpawnKeyActor()
{
	if (keyActor)
	{
		FActorSpawnParameters spawnParam;
		GetWorld()->SpawnActor<AActor>(keyActor,targetLocation, FRotator(0.0f),spawnParam);
	}
}


void AMainPlayer::SetWeaponHold(AWeaponPickup* weapon)
{
	if (weaponHold)
	{
		weaponHold->Destroy();
	}
	weaponHold = weapon;
}


// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	mainPlayerControler = Cast<AMainPlayerController>(GetController());
	//获取场景中所有的金币
	if (coinActor)
	{
		TArray<AActor*>coinArray;
		UGameplayStatics::GetAllActorsOfClass(this,coinActor,coinArray);
		if (coinArray.Num() > 0)
		{
			allCoinNum = coinArray.Num();
		}
	}
	//获取场景中所有的target
	if (targetActor)
	{
		TArray<AActor*>targetArray;
		UGameplayStatics::GetAllActorsOfClass(this, targetActor,targetArray);
		if (targetArray.Num() > 0)
		{
			targetLocation = targetArray[0]->GetActorLocation();
		}
	}
}

void AMainPlayer::MoveForward(float Value)
{
	if (!PlayerAlive()|| bHasGameWin) return; 
	bForwardMove = false;
	if ((Controller != nullptr) && (Value != 0.0f)&& (!bAttacking))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		bForwardMove = true;
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if (!PlayerAlive() || bHasGameWin) return;
	bRightMove = false;
	if ((Controller != nullptr) && (Value != 0.0f)&& (!bAttacking))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		bRightMove = true;
	}
}

void AMainPlayer::TurnAtRate(float Rate)
{
	//if (!PlayerAlive()) return;
	if (Rate)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	
}

void AMainPlayer::LookUpAtRate(float Rate)
{
	//if (!PlayerAlive()) return;
	if(Rate)
		{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		}
	
}

void AMainPlayer::Jump()
{
	Super::Jump();
	if (!PlayerAlive() && (!bHasGameWin)) return;
}




// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRMouthdown&&PlayerAlive() && (!bHasGameWin))
		{
			
			if (bForwardMove || bRightMove)
			{
			
				if (Stamina >= 1.0f)
				{
					SetMineState(EMainPlayerState::MPS_SPRINT);
					DecreaseStamina(SprintLoss * DeltaTime);
				}
				else
				{
					SetMineState(EMainPlayerState::MPS_NORMAL);
				}
			}
			else
			{
				SetMineState(EMainPlayerState::MPS_NORMAL);
			}
		
		}
	else
		{
			if (!PlayerAlive()) return;
			SetMineState(EMainPlayerState::MPS_NORMAL);
			IncreaseStamina(SprintLoss * DeltaTime);
		}
	
	if (bInterpToEnermy && enermyTarget && PlayerAlive())
	{
		FRotator rotTarget = GetLookAtRotationYaw(enermyTarget->GetActorLocation());
		FRotator rotTargetplayer = FMath::RInterpTo(GetActorRotation(), rotTarget,DeltaTime,interpSpeed);
		SetActorRotation(rotTargetplayer);
	}

	if (bHasEnermyTarget && enermyTarget)
	{
		enermyTargetLocation = enermyTarget->GetActorLocation();
		if (mainPlayerControler)
		{
			mainPlayerControler->SetEnermyLocation(enermyTargetLocation);
		}
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("sprint", IE_Pressed, this, &AMainPlayer::BSprintUp);
	PlayerInputComponent->BindAction("sprint", IE_Released, this, &AMainPlayer::BSprintDown);
	PlayerInputComponent->BindAction("LMouseDown", IE_Pressed, this, &AMainPlayer::LMouseUp);
	PlayerInputComponent->BindAction("LMouseDown", IE_Released, this, &AMainPlayer::LMouseDown);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainPlayer::LookUpAtRate);

	
}


void AMainPlayer::SetMineState(EMainPlayerState state)
{
	mainPlayerState = state;
	if (mainPlayerState == EMainPlayerState::MPS_SPRINT)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}


void AMainPlayer::IncreaseHealth(float amount)
{
	if (Health + amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += amount;
	}
}

void AMainPlayer::DecreaseHealth(float amount)
{
	if (Health - amount <= 0.0f)
	{
		Health = 0.0f;
		Die();
	}
	else
	{
		Health -= amount;
	}
}

void AMainPlayer::IncreaseCoin()
{
	CoinCount += 1;
	if (CoinCount == allCoinNum)
	{
		SpawnKeyActor();
	}
}

void AMainPlayer::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("AMainPlayer::Die"));
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst && attackMontage)
	{
		animInst->Montage_Play(attackMontage);
		animInst->Montage_JumpToSection("Death", attackMontage);
		GameLose();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (weaponHold)
	{
		weaponHold->sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->skeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->skeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponHold->attackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMainPlayer::IncreaseStamina(float amount)
{
	if (Stamina + amount >= MaxStamina)
	{
		Stamina = MaxStamina;
	}
	else
	{
		Stamina += amount;
	}
}

void AMainPlayer::DecreaseStamina(float amount)
{
	if (Stamina - amount <= 0.0f)
	{
		Stamina = 0.0f;
	}
	else
	{
		Stamina -= amount;
	}
}

void AMainPlayer::BSprintUp()
{
	bRMouthdown = true;
}

void AMainPlayer::BSprintDown()
{
	bRMouthdown = false;
}

float AMainPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Health - Damage <= 0.0f)
	{
		Health = 0.0f;
		SetMineState(EMainPlayerState::MPS_DEAD);
		Die();
	}
	else
	{
		Health -= Damage;
	}
	return Damage;
}

bool AMainPlayer::PlayerAlive()
{
	return mainPlayerState != EMainPlayerState::MPS_DEAD;
}

void AMainPlayer::NotifyAnim_DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMainPlayer::GameWin()
{
	SetHasGame(true);
	DisableInput(mainPlayerControler);
	float timeDelay = 2.5f;
	GetWorldTimerManager().SetTimer(winTimerHandle, this, &AMainPlayer::ShowWinWidget, timeDelay);
	GetWorldTimerManager().ClearTimer(loseTimerHandle);
}

void AMainPlayer::GameLose()
{
	SetHasGame(false);
	DisableInput(mainPlayerControler);
	float timeDelay = 2.5f;
	GetWorldTimerManager().SetTimer(loseTimerHandle, this, &AMainPlayer::ShowLoseWidget, timeDelay);
	GetWorldTimerManager().ClearTimer(winTimerHandle);
}

void AMainPlayer::ShowWinWidget()
{
	mainPlayerControler->DisplayWinGameWidget();
	mainPlayerControler->bShowMouseCursor = true;
	if (winSound)
	{
		UGameplayStatics::PlaySound2D(this,winSound);
	}
}

void AMainPlayer::ShowLoseWidget()
{
	mainPlayerControler->DisplayLoseGameWidget();
	mainPlayerControler->bShowMouseCursor = true;
	if (loseSound)
	{
		UGameplayStatics::PlaySound2D(this,loseSound);
	}
}