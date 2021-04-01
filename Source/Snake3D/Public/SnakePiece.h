// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTag.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SnakePiece.generated.h"

#define SPEED 2

UCLASS()
class SNAKE3D_API ASnakePiece : public AActor
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
	void AddMovementTag(AMovementTag* tag);

	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	FGameplayTag Tag;
	bool isLast;

private:

	float Radius;
	TArray<AMovementTag*> movementQueue;
	void MoveForward(float AxisValue);
	void UpdateRotation(FRotator rotation);
	void CheckRotation();
	void SpecialMove();
};
