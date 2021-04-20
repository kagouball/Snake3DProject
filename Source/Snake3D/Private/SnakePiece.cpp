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
	SpecialMove();
}

void ASnakePiece::MoveForward(float AxisValue)
{
	SetActorLocation(GetActorLocation() + GetVelocityVector());
}

void ASnakePiece::UpdateRotation(FRotator rotation) {
	FQuat QuatRotation = FQuat(rotation);
	SetActorRotation(QuatRotation, ETeleportType::None);
}

void ASnakePiece::AddMovementTag(AMovementTag* tag)
{
	/*
	int length = movementQueue.Num();
	if (length == 0) {
		movementQueue.Add(tag);
	}
	else {
		for (int i = length - 1; i > 0; --i) {
			if (movementQueue[i]->id < tag->id) {
				movementQueue.Insert(tag, i);
			}
		}
	}*/
	movementQueue.Add(tag);
	movementQueue.Sort([](const AMovementTag& t1, const AMovementTag& t2) {return t1.id < t2.id; });
	//Attention ici: Sort le tableau a chaque ajout de tag, est ce uen bonne idée niveau perf?
}

void ASnakePiece::SpecialMove() {
	if (movementQueue.Num() == 0) {
		MoveForward(SPEED);
		return;
	}
	//vecteur de mouvement
	/*Calcul inutile ici!!
	* On calcule la distance entre 2 tag pour tout les snakepiece de la chaine
	* Autant enregistrer celle ci (qui est fixe) dans le tagMovement
	*/
	FVector moveVector = GetVelocityVector();
	FVector newPos = GetActorLocation();
	while (movementQueue.Num() > 0) {
		//Tag le plus proche du centre
		AMovementTag* tag = movementQueue[0];
		if (movementQueue.Num() > 1) {
			AMovementTag* tagNext = movementQueue[1];
			if (tag->id > tagNext->id) {
				UE_LOG(LogTemp, Warning, TEXT("rip"));
			}
		}
		//Distance entre la pos du piece et le tag
		float distanceLocTagSquared = FVector::DistSquared(newPos, tag->GetActorLocation()); // economie de sqrt
		FVector* tagDir = new FVector(tag->GetActorLocation().X - newPos.X,
			tag->GetActorLocation().Y - newPos.Y,
			tag->GetActorLocation().Z - newPos.Z);
		float dotProduct = (FVector::DotProduct(tagDir->GetSafeNormal(), GetActorForwardVector().GetSafeNormal())); // -1<=value<=1 to know if vector direction are opposite
		if (dotProduct >= 0.8f)
		{
			if (distanceLocTagSquared <= moveVector.SizeSquared()) 
			{
				//Si le tag dans le vect mouv
				//Distance qu'il reste a parcourir apres le premier deplacement
				float remainingDistance = moveVector.Size() - FMath::Sqrt(distanceLocTagSquared);
				UpdateRotation(tag->GetActorRotation());	//Rotation
				newPos = tag->GetActorLocation();
				moveVector = (GetActorForwardVector().GetSafeNormal() * remainingDistance);
				DestroyFirstMovementTag();
			}
			else
			{
				break;
			}
		}
		else 
		{
			//UE_LOG(LogTemp, Warning, TEXT("rot : %s \ndotP : %f"), *(GetActorRotation()-tag->GetActorRotation()).ToString(), dotProduct);
			DestroyFirstMovementTag();
		}
	}
	SetActorLocation(newPos + moveVector);
}

void ASnakePiece::DestroyFirstMovementTag() {
	if (movementQueue.Num() == 0)
	{
		return;
	}

	AMovementTag* tagToDestroy = movementQueue[0];
	movementQueue.RemoveAt(0);
	if (isLast) 
	{
		tagToDestroy->Destroy();
	}
}

FVector ASnakePiece::GetVelocityVector() 
{
	float deltaSeconds = GetWorld()->GetDeltaSeconds();
	if (deltaSeconds == 0.f) {
		return FVector::ZeroVector;
	}
	float frameRate = 1.f / deltaSeconds;
	if (frameRate == 0.f) {
		return FVector::ZeroVector;
	}

	return GetActorForwardVector() * (SPEED * 100.f) / frameRate;
}