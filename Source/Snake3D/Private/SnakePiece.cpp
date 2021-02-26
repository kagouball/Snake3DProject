// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePiece.h"
#include "Components/SphereComponent.h"
#include "MyPawnMovementComponent.h"
#include "SnakeHead.h"
 

// Sets default values
ASnakePiece::ASnakePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isLast = true;
	Radius = 20;

	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(Radius);
	SphereComponent->SetCollisionProfileName(TEXT("ActorSpere"));

	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -Radius));
		SphereVisual->SetWorldScale3D(FVector(Radius / 50));
		SphereVisual->SetCollisionProfileName(TEXT("OverlapAll"));
	}
	/*
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionProfileName(TEXT("OverlapAll")); //for the camera not to be stack in the object when it pass in front of

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.f));
		VisualMesh->SetWorldScale3D(FVector(0.7f));
	}
	*/

	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

	Tags.Add(FName("Snake.Piece"));
}

// Called when the game starts or when spawned
void ASnakePiece::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(2);
	CheckRotation();
}

void ASnakePiece::MoveForward(float AxisValue)
{
	FVector loc = GetActorLocation();
	loc += GetActorForwardVector() * AxisValue;
	SetActorLocation(loc);
	/*
	OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("ForwardVec Piece: %s"), *GetActorForwardVector().ToString());
	*/
}

void ASnakePiece::UpdateRotation(float PitchValueG, float YawValueG, float RollValueG) {
	YawValue = YawValueG;
	RollValue = RollValueG;
	PitchValue = PitchValueG;
	FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));
	//AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None); //Manière 1 d'update la rotation d'une piece
	SetActorRotation(QuatRotation, ETeleportType::None);
}

void ASnakePiece::AddMovementTag(AMovementTag* tag)
{
	//UE_LOG(LogTemp, Warning, TEXT("Add"));
	movementQueue.Add(tag);
}

void ASnakePiece::CheckRotation() {
	if (movementQueue.Num() > 0) {
		//UE_LOG(LogTemp, Warning, TEXT("CheckRotation > 0"));
		AMovementTag* tag = movementQueue[0];
		if (FVector::Distance(GetActorLocation(), tag->GetActorLocation()) < 1.f) {
			//UE_LOG(LogTemp, Warning, TEXT("SAME LOCATION"));
			UpdateRotation(tag->PitchValue, tag->YawValue, tag->RollValue);
			SetActorLocation(tag->GetActorLocation());
			//UE_LOG(LogTemp, Warning, TEXT("num : %d"), movementQueue.Num());
			movementQueue.RemoveAt(0);
			//UE_LOG(LogTemp, Warning, TEXT("num After pop : %d"), movementQueue.Num());
			if (isLast) {
				tag->Destroy();
			}
		}
	}
}
