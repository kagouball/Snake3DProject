// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakePlayer.h"
#include "Spawner.h"
#include "GameDirector.generated.h"

UCLASS()
class SNAKE3D_API AGameDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//to clean
	ASnakePiece* SpawnSnakePiece(FVector Location, FRotator Rotation);
	AMovementTag* SpawnMovementTag(FVector Location, FRotator Rotation);
	AFood* SpawnFood(FVector Location, FRotator Rotation);

	AFood* GetClosestFoodTo(FVector Location);
	void FoodHasBeenEaten(AFood* food);

private:

	/*Food Part*/
	UPROPERTY(EditAnywhere)
		APlayField* playField;

	/*Player Part*/
	UPROPERTY(EditAnywhere)
		float startSpeed;
	UPROPERTY(EditAnywhere)
		TArray<ASnakePlayer*> players;
	void InitPlayers();
	
	/*Spawner Part*/
	UPROPERTY(EditAnywhere)
		ASpawner* spawner;
};
