// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMovment.h"

// Sets default values
ATestMovment::ATestMovment()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestMovment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestMovment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestMovment::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

