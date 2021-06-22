// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Food.h"
#include "PlayField.generated.h"


class AGameDirector;
/**
 * Represente the field of the game 
 * It is use to spawn the food
 */
UCLASS()
class SNAKE3D_API APlayField : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	//void SpawnFood();


public:

	APlayField();

	UFUNCTION()
		void SpawnNextFood();

	UFUNCTION()
		void NextFood();

	void MoveFood(AFood* foodToMove);

	void SetGameDirector(AGameDirector* gd);

private:

	FVector GetRandomPosition();

	TArray<AFood*> foods;

	AGameDirector* gameDirector;
};
