// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayer.h"
#include "GameDirector.h"

// Sets default values
ASnakePlayer::ASnakePlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//playerSpeed = startPlayerSpeed;
	
}

// Called when the game starts or when spawned
void ASnakePlayer::BeginPlay()
{
	Super::BeginPlay();
	head->SetPlayer(this);
	playerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void ASnakePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASnakePlayer::UpdatePlayerSpeed(float newSpeed)
{
	playerSpeed = newSpeed;
}

void ASnakePlayer::SetGameDirector(AGameDirector* gd)
{
	gameDirector = gd;
}

void ASnakePlayer::EatFood(AFood* food)
{
	/*
	//Add piece in snake
	AddPiece();
	//notify field that a food as been eaten
	if (Field) {
		Field->NextFood();
	}
	//Update player score
	if (playerHUD)
	{
		Score += 1;
		PlayerHUD->UpdateScore(Score);
	}
	*/
}

