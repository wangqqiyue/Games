#pragma once
#include "olcPixelGameEngine.h"

class Asteroid:public olc::PixelGameEngine
{
public:
	Asteroid() {
		sAppName = "Asteroid";
	}
	bool OnUserCreate()override {
		return true;
	};

	bool OnUserUpdate(float fElapsedTime)override{
		return true;
	};
};

