// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDirector.h"

// Sets default values
AGameDirector::AGameDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AGameDirector::BeginPlay()
{
	Super::BeginPlay();
	playField->SetGameDirector(this);
	InitPlayers();
}

// Called every frame
void AGameDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASnakePiece* AGameDirector::SpawnSnakePiece(FVector Location, FRotator Rotation)
{
	if (spawner)
	{
		return spawner->SpawnSnakePiece(Location, Rotation);
	}
	return nullptr;
}

AMovementTag* AGameDirector::SpawnMovementTag(FVector Location, FRotator Rotation)
{
	if (spawner)
	{
		return spawner->SpawnMovementTag(Location, Rotation);
	}
	return nullptr;
}

AFood* AGameDirector::SpawnFood(FVector Location, FRotator Rotation)
{
	if (spawner)
	{
		return spawner->SpawnFood(Location, Rotation);
	}
	return nullptr;
}

AFood* AGameDirector::GetClosestFoodTo(FVector Location)
{
	if (playField)
	{
		return playField->GetClosestFood(Location);
	}
	return nullptr;
}

void AGameDirector::FoodHasBeenEaten(AFood* food)
{
	//food to reuse in the future, don't worrie
	playField->NextFood();
}

void AGameDirector::InitPlayers()
{
	for(int i = 0; i<players.Num(); i++)
	{
		ASnakePlayer* player = players[i];
		player->SetGameDirector(this);
		player->UpdatePlayerSpeed(startSpeed);
	}
}

