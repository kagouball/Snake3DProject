// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayField.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Food.h"

APlayField::APlayField()
{
	// Our root component will be a sphere that reacts to physics
	//UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	//RootComponent = BoxComponent;
	//BoxComponent->InitBoxExtent(FVector(100, 100, 100));
	//BoxComponent->SetCollisionProfileName("Trigger");
	//BoxComponent->AttachTo(RootComponent);
}

void APlayField::BeginPlay() {
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::White, true, -1, 0, 10);
}

void APlayField::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
}

void APlayField::SpawnNextFood()
{
	if (Food) {
		FActorSpawnParameters SpawnParams;
		FVector size = GetComponentsBoundingBox().GetExtent();
		FVector mid = GetActorLocation();
		FVector newPos = FVector(FMath::RandRange(mid.X-size.X, mid.X + size.X),
			FMath::RandRange(mid.Y - size.Y, mid.Y + size.Y),
			FMath::RandRange(mid.Z - size.Z, mid.Z + size.Z));
		AFood* SpawnedFoodRef = GetWorld()->SpawnActor<AFood>(Food, newPos, GetActorRotation(), SpawnParams);
	}
}
