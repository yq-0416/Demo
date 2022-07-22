// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"





void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (userWidgetAsset)
	{
		playerWidget = CreateWidget(this, userWidgetAsset);
		if (playerWidget)
		{
			playerWidget->AddToViewport();
			playerWidget->SetVisibility(ESlateVisibility::Visible);
		}

	}
	if (enermyhealthWidgetAsset)
	{
		enermyhealthWidget = CreateWidget(this, enermyhealthWidgetAsset);
		if (enermyhealthWidget)
		{
			enermyhealthWidget->AddToViewport();
			enermyhealthWidget->SetVisibility(ESlateVisibility::Hidden);
			FVector2D alignInViewport(0.0f, 0.0f);
			enermyhealthWidget->SetAlignmentInViewport(alignInViewport);
		}

	}

	if (winGameWidgetAsset)
	{
		winGameWidget = CreateWidget(this, winGameWidgetAsset);
		if (winGameWidget)
		{
			winGameWidget->AddToViewport();
			winGameWidget->SetVisibility(ESlateVisibility::Hidden);
			FVector2D alignInViewport(0.0f, 0.0f);
			winGameWidget->SetAlignmentInViewport(alignInViewport);
		}

	}

	if (loseGameWidgetAsset)
	{
		loseGameWidget = CreateWidget(this, loseGameWidgetAsset);
		if (loseGameWidget)
		{
			loseGameWidget->AddToViewport();
			loseGameWidget->SetVisibility(ESlateVisibility::Hidden);
			FVector2D alignInViewport(0.0f, 0.0f);
			loseGameWidget->SetAlignmentInViewport(alignInViewport);
		}

	}
}

void AMainPlayerController::DisplayEnermyHealthBar()
{
	if (enermyhealthWidget)
	{
		bEnermyHealthBarVisible = true;
		enermyhealthWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnermyHealthBar()
{
	if (enermyhealthWidget)
	{
		bEnermyHealthBarVisible = false;
		enermyhealthWidget->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (enermyhealthWidget)
	{
		FVector2D positionInViewport;
		ProjectWorldLocationToScreen(enemyLocation, positionInViewport);
		positionInViewport.Y += 80.0f;
		FVector2D sizeInViewport(300.0f, 25.0f);
		enermyhealthWidget->SetPositionInViewport(positionInViewport);
		enermyhealthWidget->SetDesiredSizeInViewport(sizeInViewport);
	}
}

void AMainPlayerController::DisplayWinGameWidget()
{
	if (winGameWidget)
	{
		bwinGameWidgetVisible = true;
		winGameWidget->SetVisibility(ESlateVisibility::Visible);
		
	}
}

void AMainPlayerController::RemoveWinGameWidget()
{
	if (winGameWidget)
	{
		bwinGameWidgetVisible =false;
		winGameWidget->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AMainPlayerController::DisplayLoseGameWidget()
{
	if (loseGameWidget)
	{
		bLoseGameWidgetVisible = true;
		loseGameWidget->SetVisibility(ESlateVisibility::Visible);

	}
}

void AMainPlayerController::RemoveLoseGameWidget()
{
	if (loseGameWidget)
	{
		bLoseGameWidgetVisible = false;
		loseGameWidget->SetVisibility(ESlateVisibility::Hidden);

	}
}
