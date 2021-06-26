// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayer.h"
#include "GameDirector.h"

// Sets default values
ASnakePlayer::ASnakePlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	score = 0;
	itemRadius = 20.f; //WARNING, PLZ GENERALIZED RADIUS
}

// Called when the game starts or when spawned
void ASnakePlayer::BeginPlay()
{
	Super::BeginPlay();
	head->SetPlayer(this);
	playerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	UpdateIndicatorTarget();
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

void ASnakePlayer::UpdateIndicatorTarget()
{
	//oula pas beau ça
	foodIndicator->SetTarget(gameDirector->GetClosestFoodTo(foodIndicator->GetActorLocation()));
}

float ASnakePlayer::GetPlayerSpeed()
{
	return playerSpeed;
}

void ASnakePlayer::SetGameDirector(AGameDirector* gd)
{
	gameDirector = gd;
}

void ASnakePlayer::EatFood(AFood* food)
{
	//Add piece in snake
	AddPiece();
	//increase player speed
	playerSpeed += 0.1f;
	//notify field that a food as been eaten
	gameDirector->FoodHasBeenEaten(food);
	//Update player score
	if (playerHUD)
	{
		score += 1;
		playerHUD->UpdateScore(score);
	}
	UpdateIndicatorTarget();
}

void ASnakePlayer::HeadMove()
{
	//care with array.Num() --> O(n) probably, change it with a varibale LastPiece and check if exist
	if (tail.Num() == 0)
	{
		return;
	}
	AMovementTag* SpawnedTagRef = gameDirector->SpawnMovementTag(head->GetActorLocation(), head->GetActorRotation());
	if (lastTagSpawned) {
		lastTagSpawned->SetNext(SpawnedTagRef);
	}
	lastTagSpawned = SpawnedTagRef;
}

void ASnakePlayer::HitPiece(ASnakePiece* piece)
{
	if (piece && tail.IndexOfByKey(piece) != 0) //check if its not the first element of the tail (issue with collision)
	{
		Kill();
	}
}

void ASnakePlayer::AddPiece()
{
	float distance = 2.f * itemRadius + 1.f;
	AActor* lastPiece;
	//care with array.Num() --> O(n) probably, change it with a varibale LastPiece and check if exist
	if (tail.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("bibop"));
		ASnakePiece* piece = tail[tail.Num()-1];
		//as we will spawn a new piece and add it to the tail, the actual last piece of the tail will no longer be the last
		piece->isLast = false;
		lastPiece = piece;
	}
	else
	{
		lastPiece = head;
	}
	FVector actualPos = lastPiece->GetActorLocation();
	FVector vec = lastPiece->GetActorForwardVector();
	FRotator rotation = lastPiece->GetActorRotation();
	FVector newPos = actualPos - (vec * distance);
	ASnakePiece* newPiece = gameDirector->SpawnSnakePiece(newPos, rotation);
	newPiece->SetPlayer(this);
	tail.Add(newPiece);
}

void ASnakePlayer::Kill()
{
	for (ASnakePiece* piece : tail) {
		piece->Destroy();
	}
	head->Kill();
}

