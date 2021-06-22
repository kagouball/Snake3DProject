// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIndicator.h"
#include "Components/SphereComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
AFoodIndicator::AFoodIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFoodIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtTarget();
}

void AFoodIndicator::LookAtTarget()
{
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetPosition->GetActorLocation()));
	RootComponent->SetWorldRotation((RootComponent->GetComponentLocation() - targetPosition->GetActorLocation()).GetSafeNormal().Rotation() + FRotator(90, 0, 0));
}

