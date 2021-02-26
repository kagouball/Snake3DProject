// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"

#include "MyPawnMovementComponent.h"
#include "MovementTag.h"
#include "PlayField.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Values
	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;
	radius = 20;
	isCameraMoving = false;
	if (!Field) {
		Field = NULL;
	}
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(radius);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -radius));
		SphereVisual->SetWorldScale3D(FVector(radius/50));
	}
	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	//listener
	OnActorBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(2);
	UpdateRotation();
}

// Called to bind functionality to input
void ASnakeHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ASnakeHead::Turn);
	PlayerInputComponent->BindAxis("TurnCameraRight", this, &ASnakeHead::TurnRightCamera);
	PlayerInputComponent->BindAxis("MoveTop", this, &ASnakeHead::MoveTop);
	PlayerInputComponent->BindAxis("Rotate", this, &ASnakeHead::Rotate);
	PlayerInputComponent->BindAction("SpawnObject", IE_Pressed, this, &ASnakeHead::ButtonPush);
	PlayerInputComponent->BindAction("AngleRight", IE_Pressed, this, &ASnakeHead::AngleRight);
	PlayerInputComponent->BindAction("AngleLeft", IE_Pressed, this, &ASnakeHead::AngleLeft);
	PlayerInputComponent->BindAction("AngleTop", IE_Pressed, this, &ASnakeHead::AngleTop);
	PlayerInputComponent->BindAction("AngleBottom", IE_Pressed, this, &ASnakeHead::AngleBottom);

}

void ASnakeHead::ButtonPush()
{
	int mult = 2*radius+1;
	FVector actualPos = GetActorLocation();
	FVector vec = GetActorForwardVector();
	FRotator rotation = GetActorRotation();
	if (LastPiece) {
		actualPos = LastPiece->GetActorLocation();
		vec = LastPiece->GetActorForwardVector();
		rotation = LastPiece->GetActorRotation();
	}
	FVector newPos = FVector(actualPos.X - mult*vec.X,actualPos.Y - mult*vec.Y,actualPos.Z - mult*vec.Z);
	SpawnPiece(newPos, rotation);
}


//Movement

UPawnMovementComponent* ASnakeHead::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ASnakeHead::MoveForward(float AxisValue)
{
	FVector loc = GetActorLocation();
	loc += GetActorForwardVector() * AxisValue;
	SetActorLocation(loc);
	//UE_LOG(LogTemp, Warning, TEXT("ForwardVec Piece: %s"), *GetActorForwardVector().ToString());
	/*
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}*/
}

void ASnakeHead::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ASnakeHead::Turn(float AxisValue)
{
	/*
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
	*/
	if (AxisValue != 0) {
		YawValue += AxisValue;
		//UpdateRotation();
	}
}

void ASnakeHead::MoveTop(float AxisValue)
{
	/*
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += AxisValue;
	SetActorRotation(NewRotation);*/
	if (AxisValue != 0) {//permet de ne pas update à tous les tick
		PitchValue += AxisValue;
		//UpdateRotation();
	}
}

void ASnakeHead::Rotate(float AxisValue)
{
	/*
	FRotator NewRotation = GetActorRotation();
	NewRotation.Roll += AxisValue;
	SetActorRotation(NewRotation);*/
	if (AxisValue != 0) {
		RollValue += AxisValue;
		//UpdateRotation();
	}
}

void ASnakeHead::UpdateRotation() {
	if (YawValue != 0 || PitchValue != 0 || RollValue != 0) {
		//Update rotation
		FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));
		AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
		//Spawn tag
		//SpawnMovementTag(PitchValue, YawValue, RollValue); //Manière 1 d'update la rotation d'une piece
		if (LastPiece) {
			SpawnMovementTag(GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
		}
		YawValue = 0;
		RollValue = 0;
		PitchValue = 0;
	}
}

void ASnakeHead::AngleTop()
{
	MoveTop(90);
}

void ASnakeHead::AngleBottom()
{
	MoveTop(-90);
}

void ASnakeHead::AngleRight()
{
	Turn(90);
}

void ASnakeHead::AngleLeft()
{
	Turn(-90);
}

//Camera movement
void ASnakeHead::TurnRightCamera(float AxisValue) {
	if (AxisValue != 0) {
		float camYaw = SpringArm->GetRelativeRotation().Yaw;
		if (FMath::Abs(camYaw + AxisValue) < 50) {
			SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, camYaw + AxisValue, 0.f));
		}
	}
	else {
		UpdateCamera();
	}
	
}

void ASnakeHead::TurnUpCamera(float AxisValue) {

}

void ASnakeHead::UpdateCamera() {
	float camYaw = SpringArm->GetRelativeRotation().Yaw;
	if (camYaw != 0.f) {
		float sign = UKismetMathLibrary::SignOfFloat(camYaw);
		float newRot = FMath::Clamp(FMath::Abs(camYaw) -1, 0.f,90.f);
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, newRot*sign, SpringArm->GetRelativeRotation().Roll));
	}
}

/*Spawner*/
void ASnakeHead::SpawnPiece(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParams;
	ASnakePiece* SpawnedActorRef = GetWorld()->SpawnActor<ASnakePiece>(Piece, Location, Rotation, SpawnParams);
	//corps.push_front(*SpawnedActorRef);
	corps.Add(SpawnedActorRef);
	if (LastPiece) {
		LastPiece->isLast = false;
	}
	LastPiece = SpawnedActorRef;
	UE_LOG(LogTemp, Warning, TEXT("score : %d"),corps.Num());	
}

void ASnakeHead::SpawnMovementTag(float Pitch, float Yaw, float Roll) {
	FActorSpawnParameters SpawnParams;
	int mult = radius;
	FVector actualPos = GetActorLocation();
	FVector vec = GetActorForwardVector();
	//FVector newPos = FVector(actualPos.X + mult * vec.X, actualPos.Y + mult * vec.Y, actualPos.Z + mult * vec.Z);
	FVector newPos = FVector(actualPos.X , actualPos.Y , actualPos.Z );
	AMovementTag* SpawnedTagRef = GetWorld()->SpawnActor<AMovementTag>(MovementTag, newPos, GetActorRotation(), SpawnParams);
	SpawnedTagRef->PitchValue = Pitch;
	SpawnedTagRef->YawValue = Yaw;
	SpawnedTagRef->RollValue = Roll;
}

void ASnakeHead::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Snake.Piece"))) {
		ASnakePiece* piece = dynamic_cast<ASnakePiece*>(OtherActor);
		if (piece && corps.IndexOfByKey(piece) != 0) //check if its not the first element of the tail (issue with collision)
		{
			Kill();
		}
	}
	else if (OtherActor->ActorHasTag(FName("Food"))) {
		AFood* food = dynamic_cast<AFood*>(OtherActor);
		HitFood(food);
	}
}

void ASnakeHead::Kill() {
	
	for (ASnakePiece* piece : corps) {
		 piece->Destroy();
	}
	Destroy();
}

void ASnakeHead::HitFood(AFood* food) {
	ButtonPush();
	food->Destroy();
	if (Field) {
		Field->SpawnNextFood();
	}
}






