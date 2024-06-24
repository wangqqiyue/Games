#pragma once

#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class IceHockey : public olc::PixelGameEngine
{
public:
	IceHockey()
	{
		// Name your application
		sAppName = "IceHockey";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		return true;
	}
};
