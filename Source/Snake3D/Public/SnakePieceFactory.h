// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFactory.h"
#include "SnakePiece.h"

/**
 * 
 */
class SNAKE3D_API SnakePieceFactory: public IGameFactory
{
public:
	SnakePieceFactory();
	~SnakePieceFactory();

	ASnakePiece* Spawn(UWorld* world, FVector Location, FRotator Rotation);

private:
	ASnakePiece toSpawn;
	TSubclassOf<AActor> piece;
};
