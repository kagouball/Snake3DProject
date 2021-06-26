// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodIndicator.h"
#include "SnakeHead.h"
#include "SnakePiece.h"
#include "PlayerHUD.h"
#include "SnakePlayer.generated.h"

class AGameDirector;
class AFood;

UCLASS()
class SNAKE3D_API ASnakePlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetPlayerSpeed();

	void UpdatePlayerSpeed(float newSpeed);
	void UpdateIndicatorTarget();
	void SetGameDirector(AGameDirector* gd);
	void EatFood(AFood* food);
	void HeadMove();
	void HitPiece(ASnakePiece* piece);

	UFUNCTION()
		void AddPiece();

private:

	float playerSpeed;
	float itemRadius;
	int score;
	/*Manually edit*/
	UPROPERTY(EditAnywhere)
		AFoodIndicator* foodIndicator;
	UPROPERTY(EditAnywhere)
		ASnakeHead* head;
	/*Particular variable*/
	TArray<ASnakePiece*> tail;
	APlayerHUD* playerHUD;
	AGameDirector* gameDirector;
	AMovementTag* lastTagSpawned;

	/*Functions*/
	void Kill();
};
