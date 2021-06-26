// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "Food.h"
#include "Angle.h"
#include "Tools.h"
#include "Components/SphereComponent.h"
#include "SnakeHead.generated.h"

class ASnakePlayer;

UCLASS()
class SNAKE3D_API ASnakeHead : public APawn
{
	GENERATED_BODY()

	
public:
	// Sets default values for this pawn's properties
	ASnakeHead();
private:
	UFUNCTION()
		void OnOverlapBegin2(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Collapse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);


	UPROPERTY()
		class UMyPawnMovementComponent* OurMovementComponent;

	UPROPERTY()
		class USpringArmComponent* SpringArm;

	float PitchValue, YawValue, RollValue;	//Rotation 
	float radius;
	bool isCameraMoving;
	bool makeAngle;
	FQuat angle;
	enum CameraPersonModes{FPS,TPS};
	CameraPersonModes personMode;
	USphereComponent* sphereComponent;

	ASnakePlayer* player;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//Head
	void MoveForward(float AxisValue);
	void Turn(float AxisValue);
	void MoveTop(float AxisValue);
	void Rotate(float AxisValue);
	void UpdateRotation();
	//Angle
	void AngleTop();
	void AngleBottom();
	void AngleRight();
	void AngleLeft();
	//Camera
	void TurnRightCamera(float AxisValue);
	void TurnUpCamera(float AxisValue);
	void UpdateCamera();
	void SwitchCameraPersonView();
	void SetCameraPersonView(CameraPersonModes mode);
	//Other
	void Kill();
	void HitFood(AFood* food);
	//Setter
	void SetPlayer(ASnakePlayer* p);
};
