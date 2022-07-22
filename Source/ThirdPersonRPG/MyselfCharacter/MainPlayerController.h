// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class THIRDPERSONRPG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere ,BlueprintReadOnly,Category = UI)
	TSubclassOf<UUserWidget>userWidgetAsset;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = UI)
	UUserWidget* playerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget>enermyhealthWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* enermyhealthWidget;

	FVector enemyLocation;
	FORCEINLINE void SetEnermyLocation(FVector location) { enemyLocation = location; }
	bool bEnermyHealthBarVisible;
	void DisplayEnermyHealthBar();
	void RemoveEnermyHealthBar();

	//游戏胜利或失败的widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<UUserWidget>winGameWidgetAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UUserWidget* winGameWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TSubclassOf<UUserWidget>loseGameWidgetAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	UUserWidget* loseGameWidget;

	bool bwinGameWidgetVisible;
	void DisplayWinGameWidget();
	void RemoveWinGameWidget();

	bool bLoseGameWidgetVisible;
	void DisplayLoseGameWidget();
	void RemoveLoseGameWidget();

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
};
