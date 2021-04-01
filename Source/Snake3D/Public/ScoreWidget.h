// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UScoreWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTScore;

	void UpdateScore(int32 Value);

	void StoreWidgetAnimations();

	UWidgetAnimation* GetAnimationByName(FName AnimationNone) const;

private:
	TMap<FName, UWidgetAnimation*> AnimationsMap;

	UWidgetAnimation* ComboFadeAnimation;
	UWidgetAnimation* ComboShakeAnimation;
};
