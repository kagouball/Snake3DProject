// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h" 
#include "GameFramework/Actor.h"
#include "Food.generated.h"

UCLASS()
class SNAKE3D_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

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
	
	float Radius;
};
