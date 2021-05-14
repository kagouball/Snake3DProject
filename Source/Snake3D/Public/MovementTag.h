// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "MovementTag.generated.h"

/**
 * A Tag placed when the head move to notice the corps of a movement 
 */
UCLASS()
class SNAKE3D_API AMovementTag : public ATriggerSphere
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	AMovementTag();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	void SetDistanceToNext(float distance);
	float GetDistanceToNext();
	void SetNext(AMovementTag* nextTag);
	AMovementTag* GetNext();
	void Kill();
	int id;

private:
	float distanceToNext;
	AMovementTag* next;
};
