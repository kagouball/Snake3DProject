// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovementTag.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SNAKE3D_API Tools
{
public:
	Tools();
	~Tools();

	static float GetSafeFramerate(float deltaSeconds);

};
