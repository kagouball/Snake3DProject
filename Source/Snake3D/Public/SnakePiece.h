// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTag.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SnakePiece.generated.h"


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

	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	FGameplayTag Tag;

	TArray<AMovementTag*> movementQueue;

	bool isLast;

	float PitchValue, YawValue, RollValue;
	float Radius;

	void MoveForward(float AxisValue);

	void UpdateRotation(float PitchValueG, float YawValueG, float RollValueG);
	void AddMovementTag(AMovementTag* tag);
	void CheckRotation();
};
