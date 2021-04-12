// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
