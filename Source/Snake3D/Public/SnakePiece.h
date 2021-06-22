// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTag.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Spawnable.h"
#include "SnakePiece.generated.h"

#define SPEED 2.f

UCLASS()
class SNAKE3D_API ASnakePiece : public AActor, public ISpawnable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakePiece();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTarget(AMovementTag* newTarget, bool isfromTrigger);
	AMovementTag* GetTarget();

	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	FGameplayTag Tag;
	bool isLast;

private:

	float Radius;
	float ActualPathLength;
	float distance_segment;
	AMovementTag* target;
	FVector startPosition;

	void MoveForward(float AxisValue);
	void UpdateRotation(FRotator rotation);
	void SpecialMove();
	FVector GetVelocityVector();
};
