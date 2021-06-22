// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APlayField* ASpawner::SpawnPlayField(FVector Location, FRotator Rotation)
{
	if (!playField) {
		UE_LOG(LogTemp, Warning, TEXT("play field not initialized"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	APlayField* SpawnedActorRef = GetWorld()->SpawnActor<APlayField>(playField, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to spawn play field"));
		return nullptr;
	}
	return SpawnedActorRef;
}

ASnakePiece* ASpawner::SpawnSnakePiece(FVector Location, FRotator Rotation)
{
	if (!snakePiece) {
		UE_LOG(LogTemp, Warning, TEXT("snake piece not initialized"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	ASnakePiece* SpawnedActorRef = GetWorld()->SpawnActor<ASnakePiece>(snakePiece, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to spawn snake piece"));
		return nullptr;
	}
	return SpawnedActorRef;
}

AMovementTag* ASpawner::SpawnMovementTag(FVector Location, FRotator Rotation)
{
	if (!movementTag) {
		UE_LOG(LogTemp, Warning, TEXT("movement tag not initialized"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	AMovementTag* SpawnedActorRef = GetWorld()->SpawnActor<AMovementTag>(movementTag, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to spawn movement tag"));
		return nullptr;
	}
	return SpawnedActorRef;
}

AFood* ASpawner::SpawnFood(FVector Location, FRotator Rotation)
{
	if (!food) {
		UE_LOG(LogTemp, Warning, TEXT("food not initialized"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	AFood* SpawnedActorRef = GetWorld()->SpawnActor<AFood>(food, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to spawn food"));
		return nullptr;
	}
	return SpawnedActorRef;
}

