// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePiece.h"
#include "Components/SphereComponent.h"
#include "MyPawnMovementComponent.h"
#include "DrawDebugHelpers.h"
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
	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
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
	//MoveForward(SPEED);
	//CheckRotation();
	SpecialMove();
}

void ASnakePiece::MoveForward(float AxisValue)
{
	float FrameRate = 1 / GetWorld()->GetDeltaSeconds();
	FVector loc = GetActorLocation();
	loc += GetActorForwardVector() * (AxisValue * 100) / FrameRate;
	SetActorLocation(loc);
}

void ASnakePiece::UpdateRotation(FRotator rotation) {
	FQuat QuatRotation = FQuat(rotation);
	SetActorRotation(QuatRotation, ETeleportType::None);
}

void ASnakePiece::AddMovementTag(AMovementTag* tag)
{
	movementQueue.Add(tag);
}

void ASnakePiece::CheckRotation() {
	if (movementQueue.Num() > 0) {
		AMovementTag* tag = movementQueue[0];
		/*
		FVector *dir = new FVector(tag->GetActorLocation().X - GetActorLocation().X, 
									tag->GetActorLocation().Y - GetActorLocation().Y, 
									tag->GetActorLocation().Z - GetActorLocation().Z);
		float dist = dir->Size();

		FVector vec = GetActorForwardVector();
		FVector actualPos = GetActorLocation();
		int mult = 50;
		FVector newPos = FVector(actualPos.X + mult * vec.X, actualPos.Y + mult * vec.Y, actualPos.Z + mult * vec.Z);
		FVector newPos2 = FVector(actualPos.X + 100 * dir->X, actualPos.Y + 100 * dir->Y, actualPos.Z + 100 * dir->Z);
		//DrawDebugLine(GetWorld(),actualPos, newPos2,FColor::Blue,false);

		float angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(newPos, newPos2)));
		UE_LOG(LogTemp, Warning, TEXT("time : %f"), GetWorld()->GetTimeSeconds());*/

		float FrameRate = 1 / GetWorld()->GetDeltaSeconds();
		float sizeVec = (GetActorForwardVector() * (SPEED * 100) / FrameRate).Size();
		int count = 0;
		for (int i = 0; i < movementQueue.Num();i++) {
			if (FVector::Distance(GetActorLocation(), movementQueue[i]->GetActorLocation()) < sizeVec)
				tag = movementQueue[i];
		}
		//UE_LOG(LogTemp, Warning, TEXT("count : %d "), count);
		if (FVector::Distance(GetActorLocation(), tag->GetActorLocation()) < sizeVec) {
			SetActorLocation(tag->GetActorLocation());
			UpdateRotation(tag->GetActorRotation());
			movementQueue.RemoveAt(0);
			if (isLast) {
				tag->Destroy();
			}
		}
	}
}

void ASnakePiece::SpecialMove() {
	if (movementQueue.Num() == 0) {
		MoveForward(SPEED);
		return;
	}

	float FrameRate = 1 / GetWorld()->GetDeltaSeconds();
	//vecteur de mouvement
	FVector moveVector = (GetActorForwardVector() * (SPEED * 100) / FrameRate);
	FVector newPos = GetActorLocation();
	while (movementQueue.Num() > 0) {
		//Tag le plus proche
		AMovementTag* tag = movementQueue[0];
		//Distance entre la pos du piece et le tag
		//float distanceLocTag = FVector::Distance(newPos, tag->GetActorLocation());
		float distanceLocTagSquared = FVector::DistSquared(newPos, tag->GetActorLocation()); // economie de sqrt
		FVector* tagDir = new FVector(tag->GetActorLocation().X - newPos.X,
			tag->GetActorLocation().Y - newPos.Y,
			tag->GetActorLocation().Z - newPos.Z);
		float dotProduct = (FVector::DotProduct(tagDir->GetSafeNormal(), GetActorForwardVector().GetSafeNormal())); // -1<=value<=1 to know if vector direction are opposite
		if (dotProduct > 0.f) {
			if (distanceLocTagSquared < moveVector.SizeSquared()) {
				//Si le tag dans le vect mouv
				//Distance qu'il reste a parcourir apres le premier deplacement
				float remainingDistance = moveVector.Size() - FMath::Sqrt(distanceLocTagSquared);
				UpdateRotation(tag->GetActorRotation());	//Rotation
				newPos = tag->GetActorLocation();
				moveVector = (GetActorForwardVector().GetSafeNormal() * remainingDistance);
				movementQueue.RemoveAt(0);
				if (isLast) {
					tag->Destroy();
				}
			}
			else {
				break;
			}
		}
		else {
			movementQueue.RemoveAt(0);
			if (isLast) {
				tag->Destroy();
			}
		}
	}
	SetActorLocation(newPos + moveVector);
}