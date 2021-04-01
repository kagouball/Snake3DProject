// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"


APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ScoreWidgetClass != nullptr)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);

		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
		}
	}
}

void APlayerHUD::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APlayerHUD::UpdateScore(int32 Value) {
	if (ScoreWidget)
	{
		ScoreWidget->UpdateScore(Value);
	}
}
