// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "SnakePiece.h"
#include "MovementTag.h"
#include "Snake3D/Public/Food.h"
#include "Angle.h"
#include "Tools.h"
#include "SnakeHead.generated.h"

#define SPEED 2.f

UCLASS()
class SNAKE3D_API ASnakeHead : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakeHead();
	//Piece to spawn
	UPROPERTY(EditDefaultsOnly, Category = "PieceSnake")
		TSubclassOf<AActor> Piece;
	//Tag to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
		TSubclassOf<ATriggerSphere> MovementTag;
private:

	UFUNCTION()
		void SpawnPiece(FVector Location, FRotator Rotation);

	UFUNCTION()
		void SpawnMovementTag();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	UPROPERTY()
		class ASnakePiece* LastPiece; //Last piece of the snake's tale

	UPROPERTY()
		class USpringArmComponent* SpringArm;

	float PitchValue, YawValue, RollValue;	//Rotation 
	float radius;
	bool isCameraMoving;
	bool makeAngle;
	FQuat angle;
	TArray<ASnakePiece*> corps;		//Snake piece list
	enum CameraPersonModes{FPS,TPS};
	CameraPersonModes personMode;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UPROPERTY(EditAnywhere)
		class APlayField* Field;

	int32 Score;
	
	//Head
	void MoveForward(float AxisValue);
	void Turn(float AxisValue);
	void MoveTop(float AxisValue);
	void Rotate(float AxisValue);
	//Angle
	void AngleTop();
	void AngleBottom();
	void AngleRight();
	void AngleLeft();
	//Camera
	void TurnRightCamera(float AxisValue);
	void TurnUpCamera(float AxisValue);
	void UpdateCamera();
	//Other
	void AddPiece();
	void SwitchCameraPersonView();
	void SetCameraPersonView(CameraPersonModes mode);
	void UpdateRotation();
	void Kill();
	void HitFood(AFood* food);
};
