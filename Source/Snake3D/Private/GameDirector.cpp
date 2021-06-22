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
}

// Called every frame
void AGameDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ASnakePiece* AGameDirector::SpawnSnakePiece(FVector Location, FRotator Rotation)
{
	return spawner->SpawnSnakePiece(Location, Rotation);
}

AMovementTag* AGameDirector::SpawnMovementTag(FVector Location, FRotator Rotation)
{
	return spawner->SpawnMovementTag(Location, Rotation);
}

AFood* AGameDirector::SpawnFood(FVector Location, FRotator Rotation)
{
	return spawner->SpawnFood(Location,Rotation);
}

