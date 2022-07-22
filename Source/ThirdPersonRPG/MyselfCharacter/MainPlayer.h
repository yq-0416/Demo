// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"
class AWeaponPickup;
class APickupBase;
class USoundCue;
class UParticleSystem;









UENUM(BlueprintType)
enum class EMainPlayerState :uint8
{
	MPS_NORMAL UMETA(DisplayName = "Normal"),
	MPS_SPRINT UMETA(DisplayName = "Sprint"),
	MPS_ATTACK UMETA(DisplayName = "Attack"),
	MPS_HURT UMETA(DisplayName = "Hurt"),
	MPS_DEAD UMETA(DisplayName = "Dead"),
	MPS_MAX UMETA(DisplayName = "DefaultMax")
};
UCLASS()
class THIRDPERSONRPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	AMainPlayer();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//角色相关数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	EMainPlayerState mainPlayerState;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PlayerDate)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	int32 CoinCount;

	void SetMineState(EMainPlayerState state);
	void IncreaseHealth(float amount);
	void DecreaseHealth(float amount);
	void IncreaseCoin();
	void Die();

	//Set sprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float SprintLoss;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float RunSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	float SprintSpeed;
	

	void IncreaseStamina(float amount);
	void DecreaseStamina(float amount);

	bool bRMouthdown;
	void BSprintUp();
	void BSprintDown();

	bool bForwardMove;
	bool bRightMove;

	//武器设置
	bool bLMBDown;
	void LMouseDown();
	void LMouseUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
	AWeaponPickup* weaponHold;
	APickupBase* pickupOverlap;

	//角色攻击
	bool bAttacking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	class UAnimMontage* attackMontage;
	void Attack();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_AttackingEnding();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UParticleSystem* hitParticle;

	//角色攻击时转向敌人
	float interpSpeed;
	bool bInterpToEnermy;
	FORCEINLINE void SetInterpToEnermy(bool binterpBool) {bInterpToEnermy = binterpBool;}
	FRotator GetLookAtRotationYaw(FVector targetVector);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	class AMyEnermy* enermyTarget;
	FORCEINLINE void SetTargetEnermy(class AMyEnermy* target) { enermyTarget = target; }

	//角色受到伤害
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	bool PlayerAlive();

	//为敌人角色添加血量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	bool bHasEnermyTarget;
	FORCEINLINE void SetHasEnermyTarget(bool target) { bHasEnermyTarget = target; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	class AMainPlayerController* mainPlayerControler;
	FVector enermyTargetLocation;

	//游戏胜利法则
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	int allCoinNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<AActor> coinActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	bool bHasKey;
	FORCEINLINE void SetHasKey(bool has) { bHasKey = has; }
	FORCEINLINE bool GetHasKey() {return bHasKey; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerDate)
	int keyNum;
	void SetKeyFunc();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<AActor>targetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<AActor>keyActor;
	FVector targetLocation;
	void SpawnKeyActor();

	//游戏胜利或失败相关逻辑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool bHasGameWin;
	FORCEINLINE void SetHasGame(bool has) { bHasGameWin = has; }
	FORCEINLINE bool GetHasGame() { return bHasGameWin; }
	FTimerHandle winTimerHandle;
	FTimerHandle loseTimerHandle;
	void GameWin();
	void GameLose();
	void ShowWinWidget();
	void ShowLoseWidget();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	class USoundBase* winSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
	class USoundBase* loseSound;



	void SetWeaponHold(AWeaponPickup* weapon);
	FORCEINLINE void SetPickupOverlap(APickupBase* pickup) { pickupOverlap = pickup; };
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeathEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	virtual void Jump() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


