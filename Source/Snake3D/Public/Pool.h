// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFactory.h"
#include "Spawnable.h"

/**
 * 
 */
class SNAKE3D_API Pool: public IGameFactory
{
public:
	Pool();
	~Pool();
	static Pool Instance();

private:
	
	TArray<USpawnable*> pool;
	static const Pool instanceUnique;

};
