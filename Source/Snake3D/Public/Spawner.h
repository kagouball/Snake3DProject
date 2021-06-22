// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayField.h"
#include "Food.h"
#include "SnakePiece.h"
#include "MovementTag.h"
#include "Spawner.generated.h"

UCLASS()
class SNAKE3D_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APlayField* SpawnPlayField(FVector Location, FRotator Rotation);
	ASnakePiece* SpawnSnakePiece(FVector Location, FRotator Rotation);
	AMovementTag* SpawnMovementTag(FVector Location, FRotator Rotation);
	AFood* SpawnFood(FVector Location, FRotator Rotation);

private:
	UPROPERTY(EditAnywhere, Category = "ToSpawn")
		TSubclassOf<AActor> playField;
	UPROPERTY(EditAnywhere, Category = "ToSpawn")
		TSubclassOf<AActor> snakePiece;
	UPROPERTY(EditAnywhere, Category = "ToSpawn")
		TSubclassOf<AActor> movementTag;
	UPROPERTY(EditAnywhere, Category = "ToSpawn")
		TSubclassOf<AActor> food;
};
