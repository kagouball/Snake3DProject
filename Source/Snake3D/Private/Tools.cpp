// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools.h"

Tools::Tools()
{
}

Tools::~Tools()
{
}

float Tools::GetSafeFramerate(float deltaSeconds) {
	if (deltaSeconds == 0.f)
	{
		return -1.f; //early return
	}
	float FrameRate = 1.f / deltaSeconds;
	if (FrameRate == 0.f)
	{
		return -1.f; //early return
	}

	return FrameRate;
};
