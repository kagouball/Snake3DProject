// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayField.h"
#include "DrawDebugHelpers.h"
#include "GameDirector.h"
#include "Math/UnrealMathUtility.h"


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
	NextFood();
}

void APlayField::SpawnNextFood()
{
	foods.Add(gameDirector->SpawnFood(GetRandomPosition(), FRotator(0)));
}

FVector APlayField::GetRandomPosition()
{
	FVector size = GetComponentsBoundingBox().GetExtent();
	FVector mid = GetActorLocation();
	FVector newPos = FVector(
		FMath::RandRange(mid.X - size.X, mid.X + size.X),
		FMath::RandRange(mid.Y - size.Y, mid.Y + size.Y),
		FMath::RandRange(mid.Z - size.Z, mid.Z + size.Z));
	return newPos;
}

void APlayField::NextFood()
{
	UE_LOG(LogTemp, Warning, TEXT("next food"));
	if (foods.Num() == 0) {
		SpawnNextFood();
	}
	else {
		MoveFood(foods[0]);
	}
}

void APlayField::MoveFood(AFood* foodToMove)
{
	UE_LOG(LogTemp, Warning, TEXT("move food"));
	FVector newPos = GetRandomPosition();
	UE_LOG(LogTemp, Warning, TEXT("distance : %f"),FVector::Distance(newPos,foodToMove->GetActorLocation()));
	//foodToMove->Destroy();
	foodToMove->SetActorLocation(newPos);
}

AFood* APlayField::GetClosestFood(FVector Location)
{
	//not good, change it later plz
	if (foods.Num() > 0) {
		return foods[0];
	}
	return nullptr;
}

void APlayField::SetGameDirector(AGameDirector* gd)
{
	gameDirector = gd;
}
