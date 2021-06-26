// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePiece.h"
#include "Components/SphereComponent.h"
#include "MyPawnMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "SnakeHead.h"
#include "SnakePlayer.h"
 

// Sets default values
ASnakePiece::ASnakePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isLast = true; //always last when created
	Radius = 20;
	ActualPathLength = 0;
	distance_segment = 0;
	target = nullptr;
	//startPos = GetActorLocation();
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

void ASnakePiece::SetTarget(AMovementTag* newTarget , bool isFromTrigger)
{
	startPosition = isFromTrigger ? GetActorLocation() : target->GetActorLocation();
	if (newTarget) {
		ActualPathLength = FVector::Distance(startPosition, newTarget->GetActorLocation());
		//ActualPathLength = isFromTrigger ? FVector::Distance(startPosition, newTarget->GetActorLocation()) : target->GetDistanceToNext();
	}
	target = newTarget;
}

void ASnakePiece::SetPlayer(ASnakePlayer* p)
{
	player = p;
}

AMovementTag* ASnakePiece::GetTarget()
{
	return target;
}

void ASnakePiece::SpecialMove() {
	if (target == nullptr) {
		MoveForward(player->GetPlayerSpeed());
		return;
	}
	//See here if you can do some optimization
	distance_segment += player->GetPlayerSpeed() * GetWorld()->GetDeltaSeconds() * 100.f;
	while (distance_segment >= ActualPathLength && target) {
		distance_segment -= ActualPathLength;
		AMovementTag* prevTarget = target;
		UpdateRotation(target->GetActorRotation());
		SetActorLocation(target->GetActorLocation());
		SetTarget(target->GetNext(),false);
		if (isLast)
		{
			prevTarget->Kill();
		}
	}
	
	if (target) {
		//float ratio = distance_segment / ActualPathLength;
		float ratio = distance_segment / FVector::Distance(startPosition, target->GetActorLocation());
		//UE_LOG(LogTemp, Warning, TEXT("ratio : %f"), r);
		SetActorLocation(FMath::Lerp(startPosition, target->GetActorLocation(), ratio));
	}
	else 
	{
		SetActorLocation(GetActorLocation() + GetVelocityVector().GetSafeNormal() * distance_segment);
		distance_segment = 0;
	}
}


FVector ASnakePiece::GetVelocityVector() 
{
	float frameRate = Tools::GetSafeFramerate(GetWorld()->GetDeltaSeconds());
	return GetActorForwardVector() * (player->GetPlayerSpeed() * 100.f) / frameRate;
}