// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "ScoreWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

	/**Primary draw call for the HUD*/
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void UpdateScore(int32 Value);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> ScoreWidgetClass;

private:
	UScoreWidget* ScoreWidget;
};
