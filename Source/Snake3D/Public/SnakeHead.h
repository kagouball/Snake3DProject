// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakePiece.h"
#include "MovementTag.h"
#include "Snake3D/Public/Food.h"
#include "GameplayTagContainer.h" 
#include "GameFramework/Pawn.h"
#include "Engine/TriggerSphere.h"
#include "SnakeHead.generated.h"

#define SPEED 1.5

UCLASS()
class SNAKE3D_API ASnakeHead : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakeHead();

	UPROPERTY(EditDefaultsOnly, Category = "PieceSnake")
		TSubclassOf<AActor> Piece;

	UPROPERTY(EditDefaultsOnly, Category = "Tag")
		TSubclassOf<ATriggerSphere> MovementTag;
private:

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnPiece(FVector Location, FRotator Rotation);

	UFUNCTION()
		void SpawnMovementTag(float Pitch, float Yaw, float Roll);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	UPROPERTY()
		class ASnakePiece* LastPiece; //Last piece of the snake's tale

	UPROPERTY(EditAnywhere)
		class APlayField* Field;
	
	UPROPERTY()
		class USpringArmComponent* SpringArm;

	float PitchValue, YawValue, RollValue;	//Rotation 
	float radius;
	bool isCameraMoving;
	//list<ASnakePiece> corps;	//Snake piece list
	TArray<ASnakePiece*> corps;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//Head
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void MoveTop(float AxisValue);
	void Rotate(float AxisValue);

	void AngleTop();
	void AngleBottom();
	void AngleRight();
	void AngleLeft();
	//Camera
	void TurnRightCamera(float AxisValue);
	void TurnUpCamera(float AxisValue);
	void UpdateCamera();

	void ButtonPush();
	void UpdateRotation();
	void Kill();
	void HitFood(AFood* food);
};
