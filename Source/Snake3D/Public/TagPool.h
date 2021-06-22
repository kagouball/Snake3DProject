// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovementTag.h"
#include "CoreMinimal.h"
#include "Pool.h"

/**
 * 
 */
class SNAKE3D_API TagPool: public Pool
{
public:
	

	void getMovementTag();
	void releaseMovementTag();

private:
	~TagPool();
	TagPool();

	//TArray<AMovementTag*> Pool;
	//static const TagPool instanceUnique;

	//static TagPool Instance();
};
