// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SNAKE3D_API Angle
{
public:
	Angle();
	~Angle();

	static FQuat GetRightAngleBottom() { return FQuat(FRotator(-90.f, 0.f, 0.f)); };
	static FQuat GetRightAngleTop() { return FQuat(FRotator(90.f, 0.f, 0.f)); };
	static FQuat GetRightAngleRight() { return FQuat(FRotator(0.f, 90.f, 0.f)); };
	static FQuat GetRightAngleLeft() { return FQuat(FRotator(0.f, -90.f, 0.f)); };
};
