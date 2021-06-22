// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePieceFactory.h"
#include "Engine/World.h" 

SnakePieceFactory::SnakePieceFactory()
{
}

SnakePieceFactory::~SnakePieceFactory()
{
}

ASnakePiece* SnakePieceFactory::Spawn(UWorld* world, FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParams;
	ASnakePiece* SpawnedActorRef = world->GetWorld()->SpawnActor<ASnakePiece>(piece, Location, Rotation, SpawnParams);
	if (SpawnedActorRef == NULL)
	{
		return nullptr;
	}
	return SpawnedActorRef;
}
