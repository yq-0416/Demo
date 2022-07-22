// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnermy.generated.h"
class UBoxComponent;
class AAIController;
class USphereComponent;
class AMainPlayer;
class UAnimMontage;
class USoundCue;
class UParticleSystem;


UENUM(BlueprintType)
enum class EEnermyState :uint8
{
	ES_IDLE UMETA(DisplayName = "IDLE"),
	ES_MoveToTarget UMETA(DisplayName = "Move_To_Target"),
	ES_ATTACK UMETA(DisplayName = "Attack"),
	ES_HURT UMETA(DisplayName = "Hurt"),
	ES_DEAD UMETA(DisplayName = "Dead"),
	ES_MAX UMETA(DisplayName = "DefaultMax")
};
UCLASS()
class THIRDPERSONRPG_API AMyEnermy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnermy();

	//角色数据相关
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enermy)
	EEnermyState enermyState;
	FORCEINLINE void SetEnermyState(EEnermyState state) { enermyState = state; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	AAIController* enermyController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	USphereComponent* aproSphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	USphereComponent* attackSphereComp;

	//追逐和攻击玩家
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bInAgroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bInAttackSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bAttacking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	UAnimMontage* attackMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	AMainPlayer* playerTarget;

	float acuTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float tickChaseTime;

	//与玩家进行打斗
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UBoxComponent* LegComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	float health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float enermyDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	USoundCue* hitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UParticleSystem* hitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	USoundCue* swingSound;

	UFUNCTION(BlueprintCallable)
	void NotifyAnim_ActiveEnermyCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeacEnermyCollison();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_EnermyPlayWing();


	UFUNCTION(BlueprintCallable)
	void NotifAnim_AttackEnd();
	void Enermy_Attack();

	//敌人延迟进攻
	FTimerHandle attackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float minAttackTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float maxAttackTime;

	//敌人伤害角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<class UDamageType> damType;
	

	//敌人受到伤害
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Die();
	UFUNCTION(BlueprintCallable)
	void NotifyAnim_DeathEnd();
	FTimerHandle deathTime;
	float deathDelay;
	void DestoryCalled();
	bool EnermyAlive();
	bool bHasVaileTarget;
	FORCEINLINE void SetHasVaildTarget(bool has) { bHasVaileTarget = has; }

	//漫游功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool bCanWander;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	bool bWandering;
	float timePause;
	float timeAccumWander;
	void FindSphereRandomLocation(FVector& randLocation);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnAproSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAproSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void OnAttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveToTarget(AMainPlayer* target);
};
