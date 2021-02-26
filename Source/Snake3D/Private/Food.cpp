// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Components/SphereComponent.h"
#include "MyPawnMovementComponent.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Radius = 10;

	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(Radius);
	SphereComponent->SetCollisionProfileName(TEXT("ActorSpere"));

	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Shapes/Food_Sphere.Food_Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterial> plane_material(TEXT("/Game/StarterContent/Props/Materials/M_Shelf.M_Shelf"));
	if (SphereVisualAsset.Succeeded() && plane_material.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -Radius));
		SphereVisual->SetWorldScale3D(FVector(Radius / 50));
		SphereVisual->SetCollisionProfileName(TEXT("OverlapAll"));

		SphereVisual->GetStaticMesh()->SetMaterial(0, plane_material.Object);
	}

	OurMovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	Tags.Add(FName("Food"));
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

