// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PlayField.generated.h"

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

public:

	APlayField();

	UPROPERTY(EditDefaultsOnly, Category = "Food")
		TSubclassOf<AActor> Food;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void SpawnNextFood();
};
